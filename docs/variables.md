# Variables

Variables in Scribble are declared with `var` and can hold numbers, floats, strings, or booleans.

```c
var x = 10;
var name = "alice";
var pi = 3.14;
var flag = true;
```

Variables can be reassigned after declaration.

```
var x = 10;
x = 20;
```

See [variables.scribble](../examples/variables.scribble) for runnable examples.

---

## Constants

Constants are declared with `const` and cannot be reassigned. Attempting to do so will throw a runtime error.

```
const max = 100;
const greeting = "hello";
```

See [constants.scribble](../examples/constants.scribble) for runnable examples.
