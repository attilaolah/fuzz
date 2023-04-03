use std::ffi::{CStr, CString};
use std::fs;

use clap::Parser;

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
        let mut gif = parse_gif(path)?;

        let mut proto = pb::Gif::new();

        let gif_version: &str;
        let v87 = CString::from_vec_with_nul(GIF87_STAMP.to_vec())?;
        let v89 = CString::from_vec_with_nul(GIF89_STAMP.to_vec())?;
        unsafe {
            // NOTE: GIF version parsing seems to be broken.
            // All images are parsed as gif89, unless DATA[GIF_VERSION_POS] = '9';
            // This would seemingly be true for e.g. GIF9??, but not GIF89a, as expected.
            gif_version = CStr::from_ptr(DGifGetGifVersion(&mut gif)).to_str()?;
        }
        proto.version = if gif_version == v87.to_str()? {
            pb::Gif_Version::GIF87
        } else if gif_version == v89.to_str()? {
            pb::Gif_Version::GIF89
        } else {
            pb::Gif_Version::UNKNOWN
        };

        fs::write(
            path_or(&self.proto, "/dev/stdout"),
            format!("{:#?}", &proto),
        )?;

        Ok(())
    }
}

fn parse_gif(path: &str) -> Result<GifFileType, Error> {
    let f = CString::new(path)?;
    let mut err: i32 = D_GIF_SUCCEEDED.try_into()?;
    let err_ptr: *mut i32 = &mut err;
    let err_message: &str;

    let gif: GifFileType;
    let gif_ptr: *mut GifFileType;

    unsafe {
        gif_ptr = DGifOpenFileName(f.as_c_str().as_ptr(), err_ptr);
    }

    if err != D_GIF_SUCCEEDED.try_into()? {
        unsafe {
            err_message = CStr::from_ptr(GifErrorString(err)).to_str()?;
        }
        return Err(Error::new(err_message.to_string()));
    }

    unsafe {
        gif = *gif_ptr;
        if DGifSlurp(gif_ptr) != GIF_OK.try_into()? {
            return Err(Error::new("failed to process image".to_string()));
        }
        if DGifCloseFile(gif_ptr, err_ptr) != GIF_OK.try_into()? {
            err_message = CStr::from_ptr(GifErrorString(err)).to_str()?;
            return Err(Error::new(err_message.to_string()));
        }
    }

    Ok(gif)
}

fn path_or<'a>(path: &'a str, dash: &'a str) -> &'a str {
    if path == "-" {
        dash
    } else {
        path
    }
}
