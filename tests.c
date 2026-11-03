#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#define STRINGSLICE_LIB_IMPLEMENTATION
#include "stringslice.h"

const char *string = "hello world";
const char *nums[] = {"1337", "-1337", "0", "1.337", "-1.337", "0."};
#define NUMS_LEN 6

#define EMPTY_SLICE strborrow("")

void test_borrow() {
   StrSlice s1 = strborrow(string);
   StrSlice s2 = strnborrow(string, 100);
   StrSlice s3 = strnborrow(string, 5);
   StrSlice s4 = EMPTY_SLICE;

   assert(s1.len == strlen(string));
   assert(s1.len == s2.len);
   for(int i = 0; i < s1.len; i++) {
      assert(s1.s[i] == string[i]);
      assert(s1.s[i] == s2.s[i]);
   }

   char hello[] = "hello";
   assert(s3.len == strlen(hello));
   for(int i = 0; i < s3.len; i++)
      assert(s3.s[i] == hello[i]);

   assert(s4.len == 0);
}

void test_compare_slice_eq() {
   char string_cpy[100];
   strcpy(string_cpy, string);
   StrSlice s1 = strborrow(string);
   StrSlice s2 = strborrow(string_cpy);

   assert(slccmp(s1, s2) == 0);
   assert(slccmp(EMPTY_SLICE, EMPTY_SLICE) == 0);
}

void test_compare_slice_lt() {
   StrSlice s1 = strborrow("abc");
   StrSlice s2 = strborrow("wxyz");
   StrSlice s3 = strborrow("abcd");

   assert(slccmp(s1, s2) < 0);
   assert(slccmp(s1, s3) < 0);
   assert(slccmp(EMPTY_SLICE, s1) < 0);
}

void test_compare_slice_gt() {
   StrSlice s1 = strborrow("wxyz");
   StrSlice s2 = strborrow("abc");
   StrSlice s3 = strborrow("wxy");

   assert(slccmp(s1, s2) > 0);
   assert(slccmp(s1, s3) > 0);
   assert(slccmp(s1, EMPTY_SLICE) > 0);
}

void test_compare_str_eq() {
   StrSlice s = strborrow(string);
   assert(slccmpstr(s, string) == 0);
   assert(slccmpstr(EMPTY_SLICE, "") == 0);
}

void test_compare_str_lt() {
   StrSlice s = strborrow("abc");
   assert(slccmpstr(s, "wxyz") < 0);
   assert(slccmpstr(s, "abcd") < 0);
   assert(slccmpstr(EMPTY_SLICE, string) < 0);
}

void test_compare_str_gt() {
   StrSlice s = strborrow("wxyw");
   assert(slccmpstr(s, "abc") > 0);
   assert(slccmpstr(s, "wxy") > 0);
}

void test_compare_n_str_eq() {
   StrSlice s = strborrow(string);
   assert(slcncmpstr(s, string, strlen(string)) == 0);
   assert(slcncmpstr(EMPTY_SLICE, "", 0) == 0);
}

void test_compare_n_str_lt() {
   char str1[] = "abc";
   char str2[] = "wxyz";
   char str3[] = "abcd";
   StrSlice s = strborrow(str1);
   assert(slcncmpstr(s, str2, strlen(str2)) < 0);
   assert(slcncmpstr(s, str3, strlen(str3)) < 0);
   assert(slcncmpstr(EMPTY_SLICE, string, strlen(string)) < 0);
}

void test_compare_n_str_gt() {
   char str1[] = "wxyz";
   char str2[] = "abc";
   char str3[] = "wxy";
   StrSlice s = strborrow(str1);
   assert(slcncmpstr(s, str2, strlen(str2)) > 0);
   assert(slcncmpstr(s, str3, strlen(str3)) > 0);
}

void test_head() {
   StrSlice s = strborrow(string);
   assert(slchead(s) == 'h');
}

void test_tail() {
   StrSlice s = strborrow(string);
   assert(slccmpstr(slctail(s), string+1) == 0);
}

void test_last() {
   StrSlice s = strborrow(string);
   assert(slclast(s) == 'd');
}

void test_init() {
   StrSlice s = strborrow(string);
   assert(slccmpstr(slcinit(s), "hello worl") == 0);
}

void test_take() {
   StrSlice s = strborrow(string);
   StrSlice take_5 = slctake(s, 5);
   StrSlice take_all = slctake(s, s.len);
   StrSlice take_more_than_all = slctake(s, s.len + 1);
   StrSlice take_from_empty = slctake(EMPTY_SLICE, 100);

   assert(slccmpstr(take_5, "hello") == 0);
   assert(slccmp(take_all, s) == 0);
   assert(slccmp(take_more_than_all, s) == 0);
   assert(slccmp(take_from_empty, EMPTY_SLICE) == 0);
}

void test_drop() {
   StrSlice s = strborrow(string);
   StrSlice drop_6 = slcdrop(s, 6);
   StrSlice drop_all = slcdrop(s, s.len);
   StrSlice drop_more_than_all = slcdrop(s, s.len + 1);
   StrSlice drop_from_empty = slcdrop(EMPTY_SLICE, 100);
   
   assert(slccmpstr(drop_6, "world") == 0);
   assert(slccmp(drop_all, EMPTY_SLICE) == 0);
   assert(slccmp(drop_more_than_all, EMPTY_SLICE) == 0);
   assert(slccmp(drop_from_empty, EMPTY_SLICE) == 0);
}

void test_subslice() {
   StrSlice s = strborrow(string);
   StrSlice sliced = slcslice(s, 1, 7);
   StrSlice sliced_too_far = slcslice(s, 1, s.len + 1);
   StrSlice slice_empty = slcslice(EMPTY_SLICE, 50, 100);
   assert(slccmpstr(sliced, "ello w") == 0);
   assert(slccmpstr(sliced_too_far, "ello world") == 0);
   assert(slccmp(slice_empty, EMPTY_SLICE) == 0);
}

void test_split() {
   StrSlice in, out, next;

   in = strborrow("1.2.3.4.5.6.7.8.9.10");
   char *expected[] = {"1","2","3","4","5","6","7","8","9","10"};
   int i = 0;
   do {
      slcsplit(in, '.', &out, &next);
      in = next;

      assert(slccmpstr(out, expected[i]) == 0);
      i++;
   } while(next.len > 0);
   assert(i == 10);

   in = strborrow("hello, world");
   int j = 0;
   do {
      slcsplit(in, '.', &out, &next);
      in = next;

      j++;
   } while(next.len > 0);
   assert(j == 1);
   assert(slccmpstr(out, "hello, world") == 0);

   char s[] = "hello again, world.";
   in = strborrow(s);
   int k = 0;
   do {
      slcsplit(in, '.', &out, &next);
      in = next;

      k++;
   } while(next.len > 0);
   assert(k == 1);
   assert(slccmp(out, slcinit(strborrow(s))) == 0);
}

void test_contains() {
   StrSlice s = strborrow(string);
   assert(slccontains(s, 'h'));
   assert(slccontains(s, 'e'));
   assert(slccontains(s, 'l'));
   assert(slccontains(s, 'l'));
   assert(slccontains(s, 'o'));
   assert(slccontains(s, ' '));
   assert(slccontains(s, 'w'));
   assert(slccontains(s, 'o'));
   assert(slccontains(s, 'r'));
   assert(slccontains(s, 'l'));
   assert(slccontains(s, 'd'));
}

void test_not_contains() {
   StrSlice s = strborrow(string);
   assert(!slccontains(s, 'q'));
   for(char c = '\0'; c < '\0' + 127; c++) // check all ascii characters
      assert(!slccontains(EMPTY_SLICE, c));
}

void test_copy() {
   StrSlice s = strborrow(string);
   char buffer[100];

   slccpy(buffer, s);
   assert(strcmp(buffer, string) == 0);

   slccpy(buffer, EMPTY_SLICE);
   assert(strcmp(buffer, "") == 0);
}

void test_n_copy() {
   StrSlice s = strborrow(string);
   char buffer1[100];
   char buffer2[6];
   char buffer3[100];
   int buffer1_len = 100;
   int buffer2_len = 6;
   int buffer3_len = 100;

   slcncpy(buffer1, s, buffer1_len);
   slcncpy(buffer2, s, buffer2_len);
   slcncpy(buffer3, EMPTY_SLICE, buffer3_len);

   assert(strcmp(buffer1, "hello world") == 0);
   assert(strcmp(buffer2, "hello") == 0);
   assert(strcmp(buffer3, "") == 0);
}

void test_parse_int() {
   bool expected[] = {true, true, true, false, false, false};
   ParseIntResult outputs[NUMS_LEN];

   for(int i = 0; i < NUMS_LEN; i++) {
      outputs[i] = slcparse_int(strborrow(nums[i]));
      assert(outputs[i].ok == expected[i]);
   }

   assert(outputs[0].n == 1337);
   assert(outputs[1].n == -1337);
   assert(outputs[2].n == 0);
   assert(!slcparse_int(EMPTY_SLICE).ok);
}

void test_parse_uint() {
   bool expected[] = {true, false, true, false, false, false};
   ParseUIntResult outputs[NUMS_LEN];

   for(int i = 0; i < NUMS_LEN; i++) {
      outputs[i] = slcparse_uint(strborrow(nums[i]));
      assert(outputs[i].ok == expected[i]);
   }

   assert(outputs[0].n == 1337);
   assert(outputs[2].n == 0);
   assert(!slcparse_uint(EMPTY_SLICE).ok);
}

void test_parse_double() {
   ParseDoubleResult outputs[NUMS_LEN];

   for(int i = 0; i < NUMS_LEN; i++) {
      outputs[i] = slcparse_double(strborrow(nums[i]));
   }

   const double tolerance = 0.000005;

   assert(-tolerance < outputs[0].x - 1337.0 && outputs[0].x - 1337.0 < tolerance);
   assert(-tolerance < outputs[1].x - (-1337.0) && outputs[1].x - (-1337.0) < tolerance);
   assert(-tolerance < outputs[2].x - 0.0 && outputs[2].x - 0.0 < tolerance);
   assert(-tolerance < outputs[3].x - 1.337 && outputs[3].x - 1.337 < tolerance);
   assert(-tolerance < outputs[4].x - (-1.337) && outputs[4].x - (-1.337) < tolerance);
   assert(-tolerance < outputs[5].x - 0.0 && outputs[5].x - 0.0 < tolerance);

   assert(!slcparse_double(EMPTY_SLICE).ok);
   assert(!slcparse_double(strborrow(string)).ok);
}

int main() {
   test_borrow();

   test_compare_slice_eq();
   test_compare_slice_lt();
   test_compare_slice_gt();
   test_compare_str_eq();
   test_compare_str_lt();
   test_compare_str_gt();
   test_compare_n_str_eq();
   test_compare_n_str_lt();
   test_compare_n_str_gt();

   test_head();
   test_tail();
   test_last();
   test_init();

   test_take();
   test_drop();
   test_subslice();

   test_split();
   
   test_contains();
   test_not_contains();

   test_copy();
   test_n_copy();

   test_parse_int();
   test_parse_uint();
   test_parse_double();

   return 0;
}
