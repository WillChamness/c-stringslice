#ifndef STRINGSLICE_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// This struct is meant to be used on the stack, but is not
// meant to be modified directly. You should consider only
// reading StrSlice.len if possible
typedef struct {
   const char *s;
   size_t len;
} StrSlice;

typedef struct {
   int n;
   int ok;
} ParseIntResult;

typedef struct {
   unsigned int n;
   int ok;
} ParseUIntResult;

typedef struct {
   double x;
   int ok;
} ParseDoubleResult;


/*
 * Creates a slice of the entire string. The string must not be freed while the
 * slice is being used.
 * @param null_terminated_str The string, which must be null terminated
 * @return A slice of the string
 */
StrSlice strborrow(const char *null_terminated_str);

/*
 * Creates a slice of the first n characters or until the null character is reached. 
 * The string must not be freed while the slice is being used.
 * @param str The string
 * @param n The number of characters to include in the slice
 * @return A slice of the string
 */
StrSlice strnborrow(const char *str, size_t n);

/*
 * Gets the first character of the slice.
 * THIS FUNCTION IS PARTIAL AND WILL CALL exit() IF THE SLICE IS EMPTY!!!
 * @param s The slice
 * @return The first character
 */
char slchead(StrSlice s);

/*
 * Gets the slice but without the first character.
 * THIS FUNCTION IS PARTIAL AND WILL CALL exit() IF THE SLICE IS EMPTY!!!
 * @param s The slice
 * @return A new slice not containing the first character
 */
StrSlice slctail(StrSlice s);

/*
 * Gets the last character of the slice.
 * THIS FUNCTION IS PARTIAL AND WILL CALL exit() IF THE SLICE IS EMPTY!!!
 * @param s The slice
 * @return The last character
 */
char slclast(StrSlice s);

/*
 * Gets the slice but without the last character.
 * THIS FUNCTION IS PARTIAL AND WILL CALL exit() IF THE SLICE IS EMPTY!!!
 * @param s The Slice
 * @return A new slice not containing the last character
 */
StrSlice slcinit(StrSlice s);

/*
 * Takes a silce of the first n characters or the entire slice,
 * whichever is smaller.
 * @param s The slice to slice further
 * @param n The number of characters to take
 * @return A new slice containing n characters
 */
StrSlice slctake(StrSlice s, size_t n);

/*
 * Takes a slice without the first n characters or returns an
 * empty slice if n is larger than the slice length.
 * @param The slice to slice further
 * @param n The number of characters to drop
 * @return A new slice containing s.len - n characters
 */
StrSlice slcdrop(StrSlice s, size_t n);

/*
 * Takes a slice of the slice or returns an empty slice.
 * if the inputs are invalid
 * @param s The slice to slice further
 * @param start The starting index of the slice, inclusive
 * @param stop The stopping index of the slice, exclusive
 */
StrSlice slcslice(StrSlice s, size_t start, size_t stop);

/*
 * Splits the slice into a new slice before the delimiter and one after
 * it. The results will be placed into *out and *next if they are not null.
 * At a high level: in == *out + delim + *next. For example, if
 * in == "123.456.78.9" and delim == '.', then *out == "123" and 
 * *next == "456.78.9".
 * @param in The input slice
 * @param delim The delimiter to calulate the slice from
 * @param out The output
 * @param next The next string to split using the delimiter
 */
void slcsplit(StrSlice in, const char *delim, StrSlice *out, StrSlice *next);

/*
 * Lexicographically compares the slices.
 * @param s1 The first slice
 * @param s2 The second slice
 * @return 0 if the slices are the same, 1 if s1 is lexicographically greater than s1, and -1 otherwise
 */
int slccmp(StrSlice s1, StrSlice s2);

/*
 * Lexicographically compares the slice with the string, not including
 * the null character.
 * @param slice The slice
 * @param null_terminated_str The string
 * @return 0 if the slice are string are the same, 1 if the slice is lexicographically greater than the string, and -1 otherwise
 */
int slccmpstr(StrSlice slice, const char *null_terminated_str);

/*
 * Lexicographically compares the slice with the first n characters
 * of the string.
 * @param slice The slice
 * @param str The string
 * @param n The number of characters in the string
 * @return 0 if the slice are string are the same, 1 if the slice is lexicographically greater than the string, and -1 otherwise
 */
int slcncmpstr(StrSlice slice, const char *str, size_t n);

/*
 * Searches the slice for the character.
 * @param s The slice to search
 * @param target The character to search for 
 * @return True if the slice contains the target; otherwise, false
 */
int slccontains(StrSlice s, char target);

/*
 * Copies all of the characters in the string to the buffer and writes
 * a null character at the end.
 * @param src The slice to copy from
 * @param dst The buffer to copy to
 */
void slccpy(char *dst, StrSlice src);

/*
 * Copies all of the characters in the string if possible and stops
 * if there is not enough space in the destination. Writes a null
 * character at the end of the string regardless.
 * @param src The slice to copy from
 * @param dst The buffer to copy to
 * @param dst_len The length of the buffer
 */
void slcncpy(char *dst, StrSlice src, size_t dst_len);

/*
 * Parses the slice to an int. If IntResult.ok is false, then
 * the value of IntResult.n cannot be trusted.
 * @param s The slice to parse
 * @return The results of the parse
 */
ParseIntResult slcparse_int(StrSlice s);

/*
 * Parses the slice to an unsigned int. If UIntResult.ok is false,
 * then the value of UIntResult.n cannot be trusted.
 * @param s The slice to parse
 * @return The results of the parse
 */
ParseUIntResult slcparse_uint(StrSlice s);

/*
 * Parses the slice to a double. If DoubleResult.ok is false, then
 * the value of DoubleResult.x cannot be trusted.
 * @param s The slice to parse
 * @returns NULL if the slice is not a valid double; otherwise, a pointer to a double
 */
ParseDoubleResult slcparse_double(StrSlice s);

/*
 * Writes all characters in the slice to stdout.
 * @param s The slice to print
 */
void slcprint(StrSlice s);

/*
 * Writes all characters in the slice to stdout, then writes a new line character.
 * @param s The slice to print
 */
void slcprintln(StrSlice s);

#ifdef STRINGSLICE_LIB_IMPLEMENTATION

StrSlice strborrow(const char *null_terminated_str) {
   StrSlice result;
   if(null_terminated_str == NULL) {
      result.s = NULL;
      result.len = 0;
   }
   else {
      size_t len = 0;
      while(null_terminated_str[len] != '\0')
         len++;

      result.s = null_terminated_str;
      result.len = len;
   }

   return result;
}

StrSlice strnborrow(const char *str, size_t n) {
   StrSlice result;
   if(str == NULL) {
      result.s = NULL;
      result.len = 0;
   }
   else {
      size_t len = 0;
      while(len < n && str[len] != '\0')
         len++;

      result.s = str;
      result.len = len;
   }

   return result;
}

char slchead(StrSlice s) {
   if(s.len == 0) {
      fputs("Unrecoverable error: slchead: cannot take the head of an empty slice\n", stderr);
      exit(1);
   }

   return s.s[0];
}

StrSlice slctail(StrSlice s) {
   if(s.len == 0) {
      fputs("Unrecoverable error: slctail: cannot take the tail of an empty slice\n", stderr);
      exit(1);
   }

   StrSlice result;
   result.s = s.s + 1;
   result.len = s.len - 1;

   return result;
}

char slclast(StrSlice s) {
   if(s.len == 0) {
      fputs("Unrecoverable error: slclast: cannot take the last character of an empty slice\n", stderr);
      exit(1);
   }

   return s.s[s.len - 1];
}

StrSlice slcinit(StrSlice s) {
   if(s.len == 0) {
      fputs("Unrecoverable error: slcinit: cannot take the init of an empty slice\n", stderr);
      exit(1);
   }

   StrSlice result;
   result.s = s.s;
   result.len = s.len - 1;

   return result;
}

StrSlice slctake(StrSlice s, size_t n) {
   StrSlice result;
   if(n > s.len) {
      result.s = s.s;
      result.len = s.len;
   }
   else {
      result.s = s.s;
      result.len = n;
   }

   return result;
}

StrSlice slcdrop(StrSlice s, size_t n) {
   StrSlice result;
   if(n > s.len) {
      result.s = s.s;
      result.len = 0;
   }
   else {
      result.s = s.s + n;
      result.len = s.len - n;
   }

   return result;
}

StrSlice slcslice(StrSlice s, size_t start, size_t stop) {
   // "123456789"[1:4]
   // take 4 -> "1234"
   // drop 1 -> "234"
   return slcdrop(slctake(s, stop), start);
}

void slcsplit(StrSlice in, const char *delim, StrSlice *out, StrSlice *next) {
   if(delim == NULL || out == NULL || next == NULL)
      return;

   size_t delim_len = 0;
   while(delim[delim_len] != '\0')
      delim_len++;

   size_t i;
   for(i = 0; i < in.len; i++) {
      StrSlice current = slcslice(in, i, i + delim_len);
      if(slccmpstr(current, delim) == 0)
         break;
   }

   if(i == in.len) {
      // "1234567".split(",.")
      //          ^
      //          i
      // *out == "1234567"
      // *next == ""
      out->s = in.s;
      out->len = in.len;
      next->s = in.s;
      next->len = 0;
   }
   else {
      // "123,.45678".split(",.")
      //     ^
      //     i
      // delim_len == 2
      // *out == "123"
      // *next == "45678"
      //
      // "12345678,.".split(",.")
      //          ^
      //          i
      // delim_len == 2
      // *out == "12345678"
      // *next == ""
      out->s = in.s;
      out->len = i;
      next->s = in.s + i + delim_len;
      next->len = in.len - i - delim_len;
   }
}

int slccmp(StrSlice s1, StrSlice s2) {
   for(size_t i = 0; i < s1.len && i < s2.len; i++) {
      if(s1.s[i] < s2.s[i])
         return -1;
      if(s1.s[i] > s2.s[i])
         return 1;
   }

   if(s1.len < s2.len)
      return -1;
   else if(s1.len > s2.len)
      return 1;
   else
      return 0;
}

int slccmpstr(StrSlice slice, const char *null_terminated_str) {
   if(null_terminated_str == NULL)
      return -1;

   size_t str_len = 0;
   while(null_terminated_str[str_len] != '\0')
      str_len++;

   for(size_t i = 0; i < slice.len && i < str_len; i++) {
      if(slice.s[i] < null_terminated_str[i])
         return -1;
      if(slice.s[i] > null_terminated_str[i])
         return 1;
   }

   if(slice.len < str_len)
      return -1;
   else if(slice.len > str_len)
      return 1;
   else
      return 0;
}


int slcncmpstr(StrSlice slice, const char *str, size_t n) {
   for(size_t i = 0; i < slice.len && i < n; i++) {
      if(slice.s[i] < str[i])
         return -1;
      if(slice.s[i] > str[i])
         return 1;
   }

   if(slice.len < n)
      return -1;
   else if(slice.len > n)
      return 1;
   else
      return 0;
}

int slccontains(StrSlice s, char target) {
   const int TRUE = 1;
   const int FALSE = 0;
   for(size_t i = 0; i < s.len; i++) {
      if(s.s[i] == target)
         return TRUE;
   }

   return FALSE;
}

void slccpy(char *dst, StrSlice src) {
   for(size_t i = 0; i < src.len; i++)
      dst[i] = src.s[i];

   dst[src.len] = '\0';
}

void slcncpy(char *dst, StrSlice src, size_t dst_len) {
   if(dst_len == 0) return;
   if(src.len == 0) {
      dst[0] = '\0';
      return;
   }

   // dont want to overflow buffer in this case
   if(src.len >= dst_len) {
      for(size_t i = 0; i < dst_len - 1; i++)
         dst[i] = src.s[i];
      dst[dst_len - 1] = '\0';
   }
   else {
      for(size_t i = 0; i < src.len; i++)
         dst[i] = src.s[i];
      dst[src.len] = '\0';
   }
}

ParseIntResult slcparse_int(StrSlice s) {
   const int TRUE = 1;
   const int FALSE = 0;
   ParseIntResult result;
   result.n = 0;
   result.ok = TRUE;

   if(s.len == 0 || (s.len == 1 && slchead(s) == '-')) {
      result.ok = FALSE;
      return result;
   }

   int pow10 = 1;
   for(size_t i = s.len - 1; i >= 1; i--) {
      char digit = s.s[i];
      if(digit < '0' || '9' < digit) {
         result.ok = FALSE;
         break;
      }
      int digit_value = digit - '0'; // is always positive
      int result_n_cpy = result.n;
      result.n += digit_value * pow10;
      pow10 *= 10;

      if(result_n_cpy > result.n) { // integer overflow occured
         result.ok = FALSE;
         break;
      }
   }

   if(result.ok) {
      if(s.s[0] == '-')
         result.n *= -1;
      else if('0' <= s.s[0] && s.s[0] <= '9')
         result.n += (s.s[0] - '0') * pow10;
      else
         result.ok = FALSE;
   }

   return result;
}

ParseUIntResult slcparse_uint(StrSlice s) {
   const int TRUE = 1;
   const int FALSE = 0;
   ParseUIntResult result;
   result.n = 0;
   result.ok = TRUE;

   if(s.len == 0) {
      result.ok = FALSE;
      return result;
   }

   unsigned int pow10 = 1;

   for(size_t i = s.len - 1; i >= 1; i--) {
      char digit = s.s[i];
      if(digit < '0' || '9' < digit) {
         result.ok = FALSE;
         break;
      }

      unsigned int digit_value = digit - '0'; // is always positive
      unsigned int result_n_cpy = result.n;
      result.n += digit_value * pow10;
      pow10 *= 10;

      if(result_n_cpy > result.n) { // integer overflow occured
         result.ok = FALSE;
         break;
      }
   }

   // manually handle i = 0 case to prevent integer underflow in above loop
   if(result.ok) {
      if('0' <= s.s[0] && s.s[0] <= '9')
         result.n += (s.s[0] - '0') * pow10;
      else
         result.ok = FALSE;
   }

   return result;
}

ParseDoubleResult slcparse_double(StrSlice s) {
   const int TRUE = 1;
   const int FALSE = 0;
   ParseDoubleResult result;
   result.x = 0;
   result.ok = TRUE;

   StrSlice integer, fraction;
   slcsplit(s, ".", &integer, &fraction);
   if(integer.len == 0 || (integer.len == 1 && integer.s[0] == '-')) {
      result.ok = FALSE;
      return result;
   }

   double integer_pow10 = 1;
   for(size_t i = integer.len - 1; i >= 1; i--) {
      char digit = integer.s[i];
      if(digit < '0' || '9' < digit) {
         result.ok = FALSE;
         return result;
      }
      int digit_value = digit - '0';
      result.x += digit_value * integer_pow10;
      integer_pow10 *= 10;
   }

   double fraction_pow10 = 0.1;
   for(size_t i = 0; i < fraction.len; i++) {
      char digit = fraction.s[i];
      if(digit < '0' || '9' < digit) {
         result.ok = FALSE;
         return result;
      }
      int digit_value = digit - '0';
      result.x += digit_value * fraction_pow10;
      fraction_pow10 *= 0.1;
   }

   if(integer.s[0] == '-')
      result.x *= -1;
   else if('0' <= integer.s[0] && integer.s[0] <= '9')
      result.x += (integer.s[0] - '0') * integer_pow10;
   else
      result.ok = FALSE;

   return result;
}

void slcprint(StrSlice s) {
   for(size_t i = 0; i < s.len; i++)
      putchar(s.s[i]);
}

void slcprintln(StrSlice s) {
   for(size_t i = 0; i < s.len; i++)
      putchar(s.s[i]);
   putchar('\n');
}

#endif // STRINGSLICE_LIB_IMPLEMENTATION

#endif // STRINGSLICE_LIB_H
