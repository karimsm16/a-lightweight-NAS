# ESP32 NAS -- SDManager Debugging Log

## Errors encountered during debugging

### 1. identifier "freeBytes" is undefined

**Cause:** Declaration/implementation mismatch or class scope lost.

**Fix:** Match declaration/definition and use uint64_t.

------------------------------------------------------------------------

### 2. Return type mismatch (uint8_t vs uint64_t)

**Cause:** Wrong return type for storage values.

**Fix:** Use uint64_t for totalBytes(), usedBytes(), freeBytes().

------------------------------------------------------------------------

### 3. member function may not be redeclared outside its class

**Cause:** Missing } or ; before the function.

**Fix:** Fix the previous function's syntax.

------------------------------------------------------------------------

### 4. expected a declaration

**Cause:** Syntax error before this line.

**Fix:** Correct the earlier syntax error.

------------------------------------------------------------------------

### 5. declaration is incompatible with ...

**Cause:** .h and .cpp signatures differ.

**Fix:** Make signatures identical.

------------------------------------------------------------------------

### 6. a type qualifier is not allowed on a nonmember function

**Cause:** Compiler lost class scope.

**Fix:** Fix the preceding syntax error.

------------------------------------------------------------------------

### 7. identifier totalBytes/usedBytes is undefined

**Cause:** Consequence of losing class scope.

**Fix:** Fix the earlier error.

------------------------------------------------------------------------

### 8. class SDManager has no member printInfo

**Cause:** Function name mismatch.

**Fix:** Use the same name everywhere.

------------------------------------------------------------------------

### 9. identifier mounted is undefined

**Cause:** Compiler not inside class scope.

**Fix:** Repair earlier syntax.

------------------------------------------------------------------------

## Lessons Learned

-   Header and source signatures must match exactly.
-   Missing braces can create many false errors.
-   Use uint64_t for SD card sizes.
-   Always fix the first compiler error first.
