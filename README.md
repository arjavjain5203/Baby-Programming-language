# Baby Programming Language 🍼💔

**The only programming language designed specifically for singles.**

welcome to **Baby**, the language that understands your relationship status. Why code in binary when you can code in *complicated feelings*?

## Features (Current)

### 1. `hope` (Variable Declaration)
Every relationship starts with a little `hope`. Declare your integers (and your intentions) here.
```baby
hope years_single = 25;
```

### 2. `dillusion` (String Declaration)
Let's be honest, that text they sent meant nothing. It was just a `dillusion`. Store your text strings here.
```baby
dillusion reply = "I'm busy this weekend";
```

### 3. `tell_me` (Print)
Communication is key (allegedly). Use `tell_me` to shout your void into the standard output.
```baby
tell_me(years_single);
tell_me(reply);
```

### 4. `then` (Newline)
Sometimes you just need some space. Use `then` to take a breath (print a newline).
```baby
tell_me("I need space");
then;
tell_me("Much better");
```

### 5. Assignment & Ghosting (Implicit Shadowing)
Sometimes you meet someone new in a specific context (scope), and they become your whole world, overshadowing everything else. But once you leave that context... they are gone.
```baby
hope ex = 1;
{
    ex = 100; // Implicitly finds a new 'ex' in this room.
    tell_me(ex); // Prints 100
}
tell_me(ex); // Back to 1. They never existed outside the club.
```

### 5. `bye` (Exit)
When you've had enough. Just leave.
```baby
bye(0);
### 6. `maybe` (If)
Are they interested? `maybe`. Conditional execution for ambiguous signals.

### 7. `ormaybe` (Else If)
Keeping your options open? `ormaybe` someone else is better.

### 8. `moveon` (Else)
If it didn't work out, it's time to `moveon`. Execute this block when all hope is lost.

### 9. `wait` (While Loop)
Sit by the phone and `wait` for them to text back. A loop that might run forever.

---

### 10. Functions
Defining relationships is hard, but defining functions shouldn't be.
```baby
hope add(hope a, hope b) {
    bye(a + b); // Returns the sum
}

hope sum = add(10, 20);
tell_me(sum);
```

---

## Future Features (Coming Soon to a Heartbreak Near You)

We differ from other languages because we promise to commit... eventually.

(Empty for now)

---

## How to Run
1.  Pour a glass of wine.
2.  Write your tragic code in a `.by` file (e.g., `temp.by`).
3.  Build and run:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ./baby ../temp.by
    ./out
    ```

*Made with 💔 by Singles, for Singles.*
