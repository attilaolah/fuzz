use clap::Parser;

use src::error::Error;
use src::gif::Args;

fn main() -> Result<(), Error> {
    Args::parse().gif_to_proto()
}
