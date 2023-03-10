#include "src/ui.h"

int main(int ac, char **av)
{
  splendor::UI ui;
  std::vector<std::string> args(av + 1, av + ac);
  ui.cli
    .get_model(args)
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