



lynis security scan details
github, herramienta libre

proteccion de acceso físico.
con grub se presiona e y se edita la entrada del menú
en la carga del kernel se añade al final init=/bin/sh para obtener privilegios previos a la carga del sistema

para volver a montar el raid como lectura y escritura

mount -r -o remount.rw /


grub-mkpasswd-pbkdf2  genera una clave para grub

/etc/grub.d/40_custom se inserta la clave:

set superusers="usuario"
password_pbkdf2 usuario <--- código generado ---->


se realiza una búsqueda de "menuentry" en el archivo y se añade justo despues de ${CLASS}
/etc/grub.d/10_linux

--users admin
