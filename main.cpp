#include "ui.h"

int main(int ac, char **av)
{
  splendor::UI ui(ac, av);
  ui.show_table();
  while (true)
  {
    ui.show_table();
    ui.interact();
  }
  return 0;
}