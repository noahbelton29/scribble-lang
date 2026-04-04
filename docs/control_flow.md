# Control Flow

## If

Executes a block if the condition is `true`. The condition must be a boolean.

```
var x = 10;

if x > 5 {
    println("big");
}
```

## If / Else

```
var x = 3;

if x > 5 {
    println("big");
} else {
    println("small");
}
```

## Else If

Chains can be as long as you need.

```
var score = 72;

if score >= 90 {
    println("A");
} else if score >= 80 {
    println("B");
} else if score >= 70 {
    println("C");
} else {
    println("F");
}
```

## While

Repeats a block as long as the condition is `true`. The condition must be a boolean.

```
var i = 0;

while i < 5 {
    println(i);
    i += 1;
}
```

## Nested Control Flow

```
var i = 0;

while i < 3 {
    if i == 1 {
        println("one");
    } else {
        println("other");
    }
    i += 1;
}
```

> **Note:** Variables declared inside an `if` or `while` block are scoped to that block and are not accessible outside it.

See [control_flow.scribble](../examples/control_flow.scribble) for runnable examples.
