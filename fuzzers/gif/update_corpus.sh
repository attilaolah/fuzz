pushd "${BUILD_WORKSPACE_DIRECTORY}/fuzzers/gif/corpus"

# Download the 50 GIFs from tenor.com.
curl --silent https://tenor.com \
  | tr '",' '\n' \
  | grep "^https://media\.tenor\.com/.*\.gif" \
  | sort -u \
  | xargs \
  curl --remote-name-all

popd
