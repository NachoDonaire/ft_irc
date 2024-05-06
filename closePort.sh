#!/bin/bash

# Comprobar si el puerto 6667 está en uso
PORT=6667
if lsof -Pi :$PORT -sTCP:LISTEN -t >/dev/null ; then
    echo "El puerto $PORT está en uso. Liberando el puerto..."
    # Encontrar el PID del proceso que está utilizando el puerto y terminarlo
    PID=$(lsof -ti :$PORT)
    kill -9 $PID
    echo "El proceso con PID $PID ha sido terminado."
else
    echo "El puerto $PORT no está en uso."
fi

