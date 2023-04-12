use std::fs;

use clap::Parser;

use crate::error::Error;
use crate::gif::to_proto::to_proto;

mod to_proto;

extern crate gif_rust_proto as pb;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
pub struct Args {
    /// GIF file
    #[arg(short, long, default_value = "-")]
    gif: String,

    /// Proto file
    #[arg(short, long, default_value = "-")]
    proto: String,

    /// Whether to zero out colour values and extension content
    #[arg(short, long, default_value_t = false)]
    zeros: bool,

    /// Seed value
    #[arg(short, long, default_value_t = 0)]
    seed: u64,
}

impl Args {
    pub fn gif_to_proto(&self) -> Result<(), Error> {
        let proto = to_proto(path_or(&self.gif, "/dev/stdin"), self.seed, self.zeros)?;

        fs::write(
            path_or(&self.proto, "/dev/stdout"),
            format!("{:#?}", &proto),
        )?;

        Ok(())
    }
}
fn path_or<'a>(path: &'a str, dash: &'a str) -> &'a str {
    if path == "-" {
        dash
    } else {
        path
    }
}
