# Linux shell

## Archivos

`minishell.cpp`	- Implementación principal de la shell, soporta ejecución de comandos y pipes.
`miprof.cpp` - Implementación del comando especial `miprof` para medir recursos. 
`miprof.h` - Cabecera que expone la función `run_miprof` para integrarla con minishell. 

## Compilación

Requiere g++ con soporte para C++17 en Linux. Puede compilar con:

g++ minishell.cpp miprof.cpp -o minishell

## Ejecución

Para iniciar la shell:

./minishell

Una vez iniciada, se pueden utilizar comandos normales de linux, como:

minishell> ls -l
minishell> echo "Hola mundo"
minishell> cat archivo.txt | grep texto

También está disponible el comando especial miprof para medir recursos:

Modo simple (ejec)
minishell> miprof ejec ls -l

Guardar en archivo (ejecsave)
minishell> miprof ejecsave log.txt sleep 2

Ejecución con límite de tiempo (ejecutar)
minishell> miprof ejecutar 3 sleep 10

## Resultados

miprof imprime estadísticas de ejecución:

Tiempo real (segundos de reloj).
Tiempo de CPU en modo usuario.
Tiempo de CPU en modo sistema.
Memoria máxima usada (MaxRSS, en KB).
