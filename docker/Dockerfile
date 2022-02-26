FROM debian:buster

RUN apt-get -y update; \
	apt-get -y upgrade; \
	apt-get -y install openssh-server \
	vim \
	weechat \
	wget;

EXPOSE 6667

CMD ["bash"]
