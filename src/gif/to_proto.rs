use std::ffi::{CStr, CString};
use std::slice::from_raw_parts;

use cc::ColorMapObject;
use cc::DGifCloseFile;
use cc::DGifGetGifVersion;
use cc::DGifOpenFileName;
use cc::DGifSlurp;
use cc::ExtensionBlock;
use cc::GifColorType;
use cc::GifErrorString;
use cc::GifFileType;
use cc::SavedImage;
use cc::APPLICATION_EXT_FUNC_CODE;
use cc::COMMENT_EXT_FUNC_CODE;
use cc::CONTINUE_EXT_FUNC_CODE;
use cc::D_GIF_SUCCEEDED;
use cc::GIF87_STAMP;
use cc::GIF89_STAMP;
use cc::GIF_OK;
use cc::GRAPHICS_EXT_FUNC_CODE;
use cc::PLAINTEXT_EXT_FUNC_CODE;

use crate::error::Error;

extern crate gif_rust_proto as pb;

pub fn to_proto(path: &str, seed: u64, zeros: bool) -> Result<pb::Gif, Error> {
    let gif_ptr = gif_parse(path)?;
    let mut gif = unsafe { *gif_ptr };

    let mut proto = pb::Gif::new();

    let v87 = CString::from_vec_with_nul(GIF87_STAMP.to_vec())?;
    let v89 = CString::from_vec_with_nul(GIF89_STAMP.to_vec())?;
    let gif_version: &str = unsafe { CStr::from_ptr(DGifGetGifVersion(&mut gif)) }.to_str()?;
    if gif_version == v87.to_str()? {
        proto.version = pb::Version::GIF87;
    } else if gif_version == v89.to_str()? {
        proto.version = pb::Version::GIF89;
    };

    proto.width = gif.SWidth.try_into()?;
    proto.height = gif.SHeight.try_into()?;

    if !gif.SColorMap.is_null() {
        proto.set_colour_map(colour_map(unsafe { *gif.SColorMap }, zeros)?);
    }

    proto.background_colour = gif.SBackGroundColor.try_into()?;
    proto.aspect_ratio = gif.AspectByte.into();

    for i in (0 as usize)..gif.ImageCount.try_into()? {
        let val: SavedImage = unsafe { *gif.SavedImages.add(i) };

        for i in (0 as usize)..val.ExtensionBlockCount.try_into()? {
            let val: ExtensionBlock = unsafe { *val.ExtensionBlocks.add(i) };

            let mut ext = pb::Extension::new();
            ext.code = if val.Function == CONTINUE_EXT_FUNC_CODE.try_into()? {
                pb::Function::CONTINUATION
            } else if val.Function == PLAINTEXT_EXT_FUNC_CODE.try_into()? {
                pb::Function::PLAIN_TEXT
            } else if val.Function == COMMENT_EXT_FUNC_CODE.try_into()? {
                pb::Function::COMMENT
            } else if val.Function == GRAPHICS_EXT_FUNC_CODE.try_into()? {
                pb::Function::GRAPHICS
            } else if val.Function == APPLICATION_EXT_FUNC_CODE.try_into()? {
                pb::Function::APPLICATION
            } else {
                pb::Function::RANDOM_FUNCTION
            };

            let size: usize = val.ByteCount.try_into()?;
            let mut data = Vec::with_capacity(size);
            data.extend_from_slice(unsafe { from_raw_parts(val.Bytes, size) });
            ext.set_content(data);

            let mut block = pb::Block::new();
            block.value = Some(pb::Block_oneof_value::ext(ext));
            proto.blocks.push(block);
        }

        let mut desc = pb::ImageDescriptor::new();
        desc.top = val.ImageDesc.Top.try_into()?;
        desc.left = val.ImageDesc.Left.try_into()?;
        desc.width = val.ImageDesc.Width.try_into()?;
        desc.height = val.ImageDesc.Height.try_into()?;
        desc.interlaced = val.ImageDesc.Interlace;

        if !val.ImageDesc.ColorMap.is_null() {
            desc.set_colour_map(colour_map(unsafe { *val.ImageDesc.ColorMap }, zeros)?);
        }

        let size = desc.width * desc.height;
        if zeros {
            desc.set_zeros(size);
        } else {
            let mut data = Vec::with_capacity(size.try_into()?);
            data.extend_from_slice(unsafe { from_raw_parts(val.RasterBits, size.try_into()?) });
            desc.set_values(data);
        }

        let mut block = pb::Block::new();
        block.value = Some(pb::Block_oneof_value::desc(desc));
        proto.blocks.push(block);
    }

    let mut block = pb::Block::new();
    let terminator = pb::Terminator::new();
    block.value = Some(pb::Block_oneof_value::terminator(terminator));
    proto.blocks.push(block);

    proto.seed = seed;

    gif_close(gif_ptr)?;

    Ok(proto)
}

fn gif_parse(path: &str) -> Result<*mut GifFileType, Error> {
    let f = CString::new(path)?;
    let mut err: i32 = D_GIF_SUCCEEDED.try_into()?;
    let err_ptr: *mut i32 = &mut err;

    let gif_ptr: *mut GifFileType = unsafe { DGifOpenFileName(f.as_c_str().as_ptr(), err_ptr) };

    if err != D_GIF_SUCCEEDED.try_into()? {
        let err_message: &str = unsafe { CStr::from_ptr(GifErrorString(err)) }.to_str()?;
        return Err(Error::new(err_message.to_string()));
    }

    if unsafe { DGifSlurp(gif_ptr) } != GIF_OK.try_into()? {
        return Err(Error::new("failed to process image".to_string()));
    }

    Ok(gif_ptr)
}

fn gif_close(gif: *mut GifFileType) -> Result<(), Error> {
    let mut err: i32 = D_GIF_SUCCEEDED.try_into()?;
    let err_ptr: *mut i32 = &mut err;
    let err_message: &str;

    if unsafe { DGifCloseFile(gif, err_ptr) } != GIF_OK.try_into()? {
        err_message = unsafe { CStr::from_ptr(GifErrorString(err)) }.to_str()?;
        return Err(Error::new(err_message.to_string()));
    }

    Ok(())
}

fn colour_map(cmap: ColorMapObject, zeros: bool) -> Result<pb::ColourMap, Error> {
    let mut proto = pb::ColourMap::new();
    proto.present = true;
    proto.colour_resolution = log_2(cmap.ColorCount.try_into()?);
    proto.bits_per_pixel = cmap.BitsPerPixel.try_into()?;
    proto.is_sorted = cmap.SortFlag;

    if zeros {
        proto.colours = Some(pb::ColourMap_oneof_colours::zeros(
            cmap.ColorCount.try_into()?,
        ));
    } else {
        let mut rgb_vals = pb::RGBValues::new();
        for i in (0 as usize)..cmap.ColorCount.try_into()? {
            let c: GifColorType = unsafe { *cmap.Colors.add(i) };

            let mut rgb = pb::RGB::new();
            (rgb.r, rgb.g, rgb.b) = (c.Red.into(), c.Green.into(), c.Blue.into());
            rgb_vals.values.push(rgb);
        }
        proto.colours = Some(pb::ColourMap_oneof_colours::values(rgb_vals));
    }

    Ok(proto)
}

fn log_2(x: u32) -> u32 {
    31 - x.leading_zeros()
}
