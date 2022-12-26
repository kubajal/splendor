#include "ui.h"

int main(int ac, char **av)
{
  splendor::UI ui(ac, av);
  while (true)
  {
    ui.display.refresh_display(ui.model);
    ui.interact();
  }
  return 0;
}