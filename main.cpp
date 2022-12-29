#include "ui.h"

int main(int ac, char **av)
{
  splendor::UI ui;
  ui.cli
    .get_model(ac, av)
    .transform([&ui](splendor::Model model)
    {
      ui.display.initialize(model);
      ui.display.refresh_display(model);
      while (true)
      {
        ui.display.interact(model);
        ui.display.refresh_display(model);
      }
      return model;
    });
  std::cerr << "Could not create game model. Aborting." << std::endl;
  return 0;
}