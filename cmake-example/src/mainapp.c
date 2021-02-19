#include <stdio.h>
#include "greetings_en.h"
#include "greetings_ja.h"
#include "greetings_vi.h"
#include "greetings_es.h"


int main(int argc, char const *argv[]) {
  greeting_en();
  greeting_ja();
  greeting_vi();
  greeting_es();
  return 0;
}
