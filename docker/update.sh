# To be called from the Docker container.
# DO NOT RUN outside of the Docker container.

# Prevent Apt from asking too many questions.
export DEBIAN_FRONTEND=noninteractive

# First do a system upgrade.
apt-get update
apt-get --yes dist-upgrade 2>&1

# Install Bazel:
# https://docs.bazel.build/versions/master/install-ubuntu.html#install-on-ubuntu
apt-get --yes install apt-transport-https curl gnupg 2>&1
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > /etc/apt/trusted.gpg.d/bazel.gpg
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" >> /etc/apt/sources.list.d/bazel.list
apt-get update
apt-get --yes install bazel 2>&1

# Install dependencies.
apt-get --yes install python-is-python3 python3-distutils pypy-enum34 libtinfo5

# Self-destruct.
rm /etc/update.sh
