FROM ioft/i386-ubuntu:14.04
MAINTAINER eyeball<hoyong2007@naver.com>

ENV user phonebook
ENV problem phonebook
ENV source phonebook.c

RUN useradd -ms /bin/bash phonebook
RUN apt-get update
RUN apt-get install socat gcc make g++ -y
RUN whoami

ADD phonebook.c /home/$user/$source
ADD phone.txt /home/$user/phone.txt
ADD flag /home/$user/flag

RUN gcc -fno-stack-protector -s -fPIE -pie -Wl,-z,relro,-z,now -o /home/$user/$problem /home/$user/$source

WORKDIR /home/$user
RUN chgrp $user /home/$user/$problem
RUN chgrp $user /home/$user/phone.txt
RUN chmod 550 /home/$user/$problem
RUN chmod 550 /home/$user/phone.txt

RUN chown -R root /home/$user/
RUN chmod 641 /home
RUN rm -rf /home/$user/.bashrc /home/$user/.profile /home/$user/.bash_logout
RUN chgrp $user /home/$user/flag
RUN chmod 640 /home/$user/phone.txt
RUN chmod 640 /home/$user/flag
RUN ls -al /home/$user/
RUN rm /home/$user/$source

USER $user
CMD socat TCP-LISTEN:5559,reuseaddr,fork EXEC:./$problem

EXPOSE 5559
