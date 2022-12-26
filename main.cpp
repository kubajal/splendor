#include "ui.h"

int main(int ac, char **av)
{
  splendor::UI ui;
  std::optional<splendor::Model> model_opt = ui.cli.get_model(ac, av);
  if (model_opt)
  {
    splendor::Model model = model_opt.value();
    ui.display.initialize(model);
    while (true)
    {
      ui.display.refresh_display(model);
      ui.display.interact(model);
    }
  }
  else
  {
    std::cerr << "Could not create game model. Aborting." << std::endl;
    return 1;
  }
  return 0;
}