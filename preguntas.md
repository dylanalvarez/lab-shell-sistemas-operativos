# Lab shell

## Parte 1: Invocación de comandos

### Búsqueda en *$PATH* ☆

- _<u>Pregunta:</u>_ ¿cuáles son las diferencias entre la *syscall* `execve(2)` y la familia de *wrappers* proporcionados por la librería estándar de *C* (*libc*) `exec(3)`?

- _<u>Respuesta:</u>_ La syscall `execve(2)` toma la ruta absoluta del archivo ejecutable, mientras que los wrappers............

## Parte 2: Invocación avanzada

### Comandos _built-in_ ☆

_<u>Pregunta</u>_: ¿entre `cd` y `pwd`, alguno de los dos se podría implementar sin necesidad de ser _built-in_? ¿por qué? ¿cuál es el motivo, entonces, de hacerlo como _built-in_? (para esta última pregunta pensar en los _built-in_ como `true` y `false`)

### Variables de entorno adicionales ☆☆

- _<u>Ayuda:</u>_ luego de llamar a `fork(2)` realizar, por cada una de las variables de entorno a agregar, una llamada a `setenv(3)`.

  _<u>Pregunta</u>:_ ¿por qué es necesario hacerlo luego de la llamada a `fork(2)` ?

- En algunas de los _wrappers_ de la familia de funciones de `exec(3)` (las que finalizan con la letra _e_), se les puede pasar un tercer argumento (o una lista de argumentos dependiendo del caso), con nuevas variables de entorno para la ejecución de ese proceso. 

  Supongamos, entonces, que en vez de utilizar `setenv(3)` por cada una de las variables, se guardan en un array y se lo coloca en el tercer argumento de una de las funciones de `exec(3)`.

  _<u>Responder (opcional):</u>_

  - ¿el comportamiento es el mismo que en el primer caso? Explicar qué sucede y por qué.
  - Describir brevemente una posible implementación para que el comportamiento sea el mismo.

### Procesos en segundo plano ☆☆☆

- Detallar cúal es el mecanismo utilizado.

## Parte 3: Redirecciones

### Flujo estándar ☆☆

  - _<u>Challenge</u>:_ investigar, describir y agregar la funcionalidad del operador de redirección **`>>`** y **`&>`**

### Tuberías simples *(pipes)* ☆☆☆

- Investigar y describir brevemente el mecanismo que proporciona la syscall `pipe(2)`, en particular la sincronización subyacente y los errores relacionados.

## Challenges promocionales

### Pseudo-variables ★

Existen las denominadas variables de entorno *mágicas*, o pseudo-variables. Estas variables son propias del *shell* (no están formalmente en *environ*) y cambian su valor dinámicamente a lo largo de su ejecución. Implementar **`?`** como única variable *mágica* (describir, también, su próposito).

- Investigar al menos otras dos variables mágicas estándar, y describir su propósito.

### Tuberías múltiples ★★

Extender el funcionamiento de la *shell* para que se puedan ejecutar *n* comandos concatenados. Describir el diseño que se utilizó para poder realizarlo.

```bash
$ ls -l | grep Doc | wc
     1       9      64
```

### Segundo plano avanzado ★★★

```bash
$ sleep 2 &
PID=2489

$ sleep 3
<pasan tres segundos>

==> terminado: PID=2489 (sleep 2)
$
```

En otras palabras, se notifica de que ha terminado un proceso en segundo plano oportunísticamente, justo antes de imprimir un *prompt* nuevamente. 

_<u>Preguntas:</u>_

- Explicar detalladamente cómo se manejó la terminación del mismo.

- ¿Por qué es necesario el uso de señales?
