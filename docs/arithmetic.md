# Arithmetic

Scribble supports the four standard arithmetic operators on integers and floats.

## Basic Operators

| Operator | Description |
|----------|-------------|
| `+`      | Addition    |
| `-`      | Subtraction |
| `*`      | Multiplication |
| `/`      | Division    |

```
var a = 10 + 5;
var b = 10 - 5;
var c = 10 * 5;
var d = 10 / 2;
```

## Floats

```
var x = 3.14 * 2.0;
var y = 10.0 / 4.0;
```

## Compound Assignment

Compound assignment operators modify a variable in place.

| Operator | Equivalent  |
|----------|-------------|
| `+=`     | `x = x + n` |
| `-=`     | `x = x - n` |
| `*=`     | `x = x * n` |
| `/=`     | `x = x / n` |

```
var x = 10;
x += 5;
x -= 3;
x *= 2;
x /= 4;
```

> **Note:** You cannot mix integers and floats in binary expressions. Use consistent types on both sides.

See [arithmetic.scribble](../examples/arithmetic.scribble) for runnable examples.
