FROM greyltc/archlinux-aur




RUN pacman -Sy base-devel --noconfirm

RUN aur-install crow  mariadb-connector-cpp-git

COPY . /usr/src/grapeshotClone

WORKDIR  /usr/src/grapeshotClone

RUN make
 
CMD [ "./bin/main" ]