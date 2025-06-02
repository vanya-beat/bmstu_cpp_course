FROM ubuntu:22.04

COPY docker/image/install_deps.sh /
RUN sh install_deps.sh
#COPY . /cpp_course
ENV TZ=Europe/Moscow

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

ENV CXX="/usr/bin/clang++-17"
ENV CC="/usr/bin/clang-17"

# SSH
EXPOSE 22
RUN echo "StrictHostKeyChecking=no" >> /etc/ssh/ssh_config
RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]
