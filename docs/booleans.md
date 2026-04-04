# Booleans & Comparison

## Boolean Literals

```
var yes = true;
var no = false;
```

## Comparison Operators

Comparison expressions evaluate to a boolean value.

| Operator | Description           |
|----------|-----------------------|
| `==`     | Equal to              |
| `!=`     | Not equal to          |
| `<`      | Less than             |
| `>`      | Greater than          |
| `<=`     | Less than or equal    |
| `>=`     | Greater than or equal |

```
var a = 10 == 10;
var b = 10 != 5;
var c = 10 > 5;
var d = 10 < 5;
var e = 10 >= 10;
var f = 5 <= 9;
```

## Using Comparisons Inline

Comparisons are expressions, so they can be used anywhere a value is expected.

```
var x = 10;
var isLarge = x > 5;

if isLarge {
    println("large");
}
```

See [booleans.scribble](../examples/booleans.scribble) for runnable examples.
