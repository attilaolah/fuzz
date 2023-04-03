use std::ffi::{CStr, CString};
use std::fs;

use clap::Parser;

use cc::ColorMapObject;
use cc::DGifCloseFile;
use cc::DGifGetGifVersion;
use cc::DGifOpenFileName;
use cc::DGifSlurp;
use cc::GifErrorString;
use cc::GifFileType;
use cc::D_GIF_SUCCEEDED;
use cc::GIF87_STAMP;
use cc::GIF89_STAMP;
use cc::GIF_OK;

use crate::error::Error;

extern crate gif_rust_proto as pb;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
pub struct Args {
    /// GIF file
    #[arg(long, default_value = "-")]
    gif: String,

    /// Proto file
    #[arg(long, default_value = "-")]
    proto: String,
}

impl Args {
    pub fn gif_to_proto(&self) -> Result<(), Error> {
        let path = path_or(&self.gif, "/dev/stdin");
        let gif_ptr = gif_parse(path)?;
        let mut gif = unsafe { *gif_ptr };

        let mut proto = pb::Gif::new();

        let v87 = CString::from_vec_with_nul(GIF87_STAMP.to_vec())?;
        let v89 = CString::from_vec_with_nul(GIF89_STAMP.to_vec())?;
        let gif_version: &str = unsafe { CStr::from_ptr(DGifGetGifVersion(&mut gif)) }.to_str()?;
        if gif_version == v87.to_str()? {
            proto.version = pb::Gif_Version::GIF87;
        } else if gif_version == v89.to_str()? {
            proto.version = pb::Gif_Version::GIF89;
        };

        proto.width = gif.SWidth.try_into()?;
        proto.height = gif.SHeight.try_into()?;

        if !gif.SColorMap.is_null() {
            let val: ColorMapObject = unsafe { *gif.SColorMap };
            let mut cmap = pb::Gif_ColourMap::new();
            cmap.present = true;
            cmap.colour_resolution = log_2(val.ColorCount.try_into()?);
            cmap.bits_per_pixel = val.BitsPerPixel.try_into()?;
            cmap.is_sorted = val.SortFlag;
            proto.set_colour_map(cmap);
        }

        //println!("{:#?}", &gif);

        fs::write(
            path_or(&self.proto, "/dev/stdout"),
            format!("{:#?}", &proto),
        )?;

        gif_close(gif_ptr)?;

        Ok(())
    }
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

fn path_or<'a>(path: &'a str, dash: &'a str) -> &'a str {
    if path == "-" {
        dash
    } else {
        path
    }
}

fn log_2(x: u32) -> u32 {
    31 - x.leading_zeros()
}
