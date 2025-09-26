# Linux Shell con Miprof

## Archivos

| Archivo         | Descripción                                                                  |
| --------------- | ---------------------------------------------------------------------------- |
| `minishell.cpp` | Implementación principal de la shell, soporta ejecución de comandos y pipes. |
| `miprof.cpp`    | Implementación del comando especial `miprof` para medir recursos.            |
| `miprof.h`      | Cabecera que expone la función `run_miprof` para integrarla con minishell.   |

---

## Compilación

Requiere `g++` con soporte para C++17 en Linux. Puede compilar con:

```bash
g++ minishell.cpp miprof.cpp -o minishell
```

---

## Ejecución

Para iniciar la shell:

```bash
./minishell
```

Una vez iniciada, se pueden utilizar comandos normales de Linux, como:

```bash
minishell> ls -l
minishell> echo "Hola mundo"
minishell> cat archivo.txt | grep texto
```

También está disponible el comando especial **miprof** para medir recursos:

* **Modo simple (ejec)**

  ```bash
  minishell> miprof ejec ls -l
  ```

* **Guardar en archivo (ejecsave)**

  ```bash
  minishell> miprof ejecsave log.txt sleep 2
  ```

* **Ejecución con límite de tiempo (ejecutar)**

  ```bash
  minishell> miprof ejecutar 3 sleep 10
  ```

---

## Resultados

`miprof` imprime estadísticas de ejecución como:

```
Comando: ls -l
Real: 0.002s, User: 0.000s, Sys: 0.001s
MaxRSS: 1240 KB
```

Cuando se utiliza `ejecsave`, estos resultados se almacenan en el archivo especificado.

