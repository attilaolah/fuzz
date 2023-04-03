use clap::Parser;

use fuzz::error::Error;
use fuzz::gif::Args;

fn main() -> Result<(), Error> {
    Args::parse().gif_to_proto()
}
