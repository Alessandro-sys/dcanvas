# Hidden Metadata Exercise - noStruct

This is an exercise to practice pointers. I'm learning C, and I asked Claude to create an exercise that would allow me to practice everything I've learned so far.

## ds

The idea is simple, create string pointers with hidden metadata behind them:

```text
+----+----+--------+--+
|clen|ccap| ...... |\0|
+----+----+--------+--+
```

Where:

* `clen` is the current length of the string;
* `ccap` is the current capacity of the string;
* `...` is the string data;
* `\0` is the null terminator.

The "user" receives a pointer to the string directly: `s + 8`, so they can use it as a normal C string. However, I know that behind that pointer there is metadata, so I can use `s - 4` to access the current capacity or `s - 8` to access the current length, both in O(1) time.

With a simple `printf("%s", s)`, I can print the string as a normal C string.

## canvas

The idea behind the canvas is similar to the `ds`:

```text
+----+----+--------+--+
|HHHH|WWWW| ...... |\0|
+----+----+--------+--+
```

The hidden metadata this time is the height and width of the canvas.

In this file, I can create custom shapes inside the canvas. I can work with `(x, y)` coordinates and transform them into one-dimensional array indices. I can also render a canvas into a `ds` to make it easier to print.

## conclusion

I know this isn't perfect C yet, but I'm still learning a lot. It was fun to learn about pointer logic, to understand why I couldn't fit a `char` into the metadata section without getting an error, and to understand how to move around inside a two-dimensional canvas represented as a one-dimensional array.

This exercise also taught me how to make the program more efficient: instead of increasing the size of a `ds` string by exactly the amount of space I need, I can double its capacity and leave some space unused. This allows me to sacrifice a bit of memory in exchange for avoiding frequent reallocations.

For example, when using the `catc` function, I would have to reallocate memory every time I add a single character if there isn't enough space. By doubling the capacity, I can fit many more characters without having to reallocate memory every time I add one.


The next exercises will be harder.
