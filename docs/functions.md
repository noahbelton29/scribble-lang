# Functions

## Declaration

Functions are declared with `func`, followed by a name, a parameter list, and a body block.

```
func greet(name) {
    println(name);
}
```

## Calling Functions

```
greet("alice");
```

## Multiple Parameters

```
func add(a, b) {
    println(a + b);
}

add(10, 5);
```

## No Parameters

```
func sayHello() {
    println("hello");
}

sayHello();
```

## Using Variables Inside Functions

Parameters and variables declared inside a function are local to that function. They do not affect or have access to variables in the outer scope.

```
var x = 100;

func show(x) {
    println(x);
}

show(42);
println(x);
```

Output:
```
42
100
```

## Calling Functions from Functions

```
func double(n) {
    println(n * 2);
}

func run() {
    double(5);
    double(10);
}

run();
```

> **Note:** Functions must be declared before they are called. Scribble does not hoist function declarations.

See [functions.scribble](../examples/functions.scribble) for runnable examples.
