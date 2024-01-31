FROM ubuntu:16.04
MAINTAINER Greyson Parrelli <greyson.parrelli@gmail.com>

# Install required packages
RUN apt-get update && apt-get install -y \
  apache2 \
  php \
  libapache2-mod-php \
  make \
  gcc

# Copy project files into the container
COPY . /root/
WORKDIR /root/

# Run the script that builds necessary tamagotchi stuff
RUN chmod 755 /root/*.sh
RUN /root/build.sh

# Entry point should run the tamagotchis
ENTRYPOINT ["/root/run.sh"]

EXPOSE 80
