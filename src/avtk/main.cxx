
#include "test_ui.hxx"

int main()
{
	TestUI* ui = new TestUI();

	ui->run();

	delete ui;

	//cairo_debug_reset_static_data();

	return 0;
}
