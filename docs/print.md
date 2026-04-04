# Print

Scribble has two built-in print statements.

| Statement  | Description                        |
|------------|------------------------------------|
| `print()`  | Prints a value with no newline     |
| `println()`| Prints a value followed by newline |

## Usage

```
print("hello ");
println("world");
println(42);
println(3.14);
println(true);
println(false);
```

Output:
```
hello world
42
3.14
true
false
```

## Printing Expressions

Any expression can be passed to `print` or `println`.

```
println(10 + 5);
println(10 == 10);

var x = 7;
println(x * 2);
```

See [print.scribble](../examples/print.scribble) for runnable examples.
