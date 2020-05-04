# <center> TP1 </center>
# <center> Protocolo DBUS </center>

### <center> BOTTA, Guido Tomas </center>

### <center> Padron: 102103 </center>

### <center> Enlace a Github: https://github.com/guidobotta/tp1-taller </center>

# Introducción

&nbsp;&nbsp;&nbsp;&nbsp; El presente trabajo practico fue realizado con la finalidad de incorporar conceptos de comunicación cliente-servidor a través de la utilización de Sockets TCP.

&nbsp;&nbsp;&nbsp;&nbsp; Se desarrollo una implementación del sistema entre procesos y llamadas a procedimientos remotos llamado D-BUS. Se implementaron tanto la aplicación de cliente como la de servidor.

# Desarrollo

## Socket TPC

&nbsp;&nbsp;&nbsp;&nbsp; Para este trabajo, se utilizó el socket tipo TPC. El tipo TPC trabaja unicamente con bytes, es decir, envía y recibe bytes. 

&nbsp;&nbsp;&nbsp;&nbsp; A diferencia de otros tipos, TCP garantiza que los bytes enviados llegaran en el mismo orden, sin repeticiones y sin pérdidas del otro lado. Sin embargo, uno de los problemas de TCP es que no garantiza que todos los bytes sean enviados en una sola llamada, por lo que se deberá recurrir a multiples llamados a la función `send`.

&nbsp;&nbsp;&nbsp;&nbsp; Para la solución a este problema, se implemento un TDA Socket, en el cual se implementaron las funciones con el objetivo de facilitar la tarea al usuario. Tanto en las funciones `send` como `rcv` se trataron los posibles problemas de no haber enviado o recibido todos los bytes deseados. La solución para el caso del `rcv` es la siguiente:

```
int socket_receive(socket_t *self, char *buffer, size_t length, int flags){
    size_t bytes_recv = 0;
    size_t status = 1;
    
    while ((bytes_recv < length) && (status != 0)){
        status = recv(self->socket, &(buffer[bytes_recv]), length, flags);
        
        if (status == -1){
            printf("Error: %s\n", strerror(errno));
            return -1;
        }

        bytes_recv += status;
    }
    
    return (int)bytes_recv;
}
```

&nbsp;&nbsp;&nbsp;&nbsp; El usuario debe pasar como parámetro el socket, buffer, longitud y flags, al igual que en la función `rcv`. Pero a diferencia de la función original, el problema de tener que realizar multiples envíos se soluciona con el `while` dentro de la función `socket_receive`.

&nbsp;&nbsp;&nbsp;&nbsp; En cuanto a la función `send`, la solución es exactamente la misma, pero tratando los bytes enviados en vez de recibidos.

## Protocolo DBUS

&nbsp;&nbsp;&nbsp;&nbsp;

## Aplicación Cliente

&nbsp;&nbsp;&nbsp;&nbsp; La aplicación cliente tiene la función de conectarse a un servidor, de ip y puerto pasados por parámetro al ejecutar, recibir uno o más mensajes, provenientes de una entrada `stdin` o de un archivo de entrada especificado en la ejecución, con un formato específico, convertir el mensaje en bytes con el formato correspondiente al protocolo DBUS, y enviar dicho mensaje formateado al servidor. Si el servidor recibió correctamente el mensaje, el cliente recibirá un mensaje `'OK\n'` proveniente de dicho servidor.

&nbsp;&nbsp;&nbsp;&nbsp; La forma de ejecución de la aplicación cliente es la siguiente:

```
./client <host> <puerto> [<archivo de entrada>]
```
>./client \<host> \<puerto> \[\<archivo de entrada>]

## Aplicación Servidor

&nbsp;&nbsp;&nbsp;&nbsp;

## Conexión Cliente-Servidor

&nbsp;&nbsp;&nbsp;&nbsp;

# Conclusiones