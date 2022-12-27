#include "ui.h"

int main(int ac, char **av)
{
  splendor::UI ui;
  ui.cli
    .get_model(ac, av)
    .transform([&ui](splendor::Model model)
    {
      ui.display.initialize(model);
      while (true)
      {
        ui.display.refresh_display(model);
        ui.display.interact(model);
      }
      return model;
    });
  std::cerr << "Could not create game model. Aborting." << std::endl;
  return 0;
}