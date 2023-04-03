use std::ffi::{FromVecWithNulError, IntoStringError, NulError};
use std::io;
use std::num::TryFromIntError;
use std::str::Utf8Error;

#[derive(Debug)]
pub enum Error {
    Error(String),
    IoError(io::Error),
    NulError(NulError),
    FromVecWithNulError(FromVecWithNulError),
    IntoStringError(IntoStringError),
    TryFromIntError(TryFromIntError),
    Utf8Error(Utf8Error),
}

impl Error {
    pub fn new(message: String) -> Self {
        Self::Error(message)
    }
}

impl From<io::Error> for Error {
    fn from(error: io::Error) -> Self {
        Error::IoError(error)
    }
}

impl From<NulError> for Error {
    fn from(error: NulError) -> Self {
        Error::NulError(error)
    }
}

impl From<FromVecWithNulError> for Error {
    fn from(error: FromVecWithNulError) -> Self {
        Error::FromVecWithNulError(error)
    }
}

impl From<TryFromIntError> for Error {
    fn from(error: TryFromIntError) -> Self {
        Error::TryFromIntError(error)
    }
}

impl From<IntoStringError> for Error {
    fn from(error: IntoStringError) -> Self {
        Error::IntoStringError(error)
    }
}

impl From<Utf8Error> for Error {
    fn from(error: Utf8Error) -> Self {
        Error::Utf8Error(error)
    }
}
