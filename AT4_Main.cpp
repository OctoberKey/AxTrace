#include "stdafx.h"

#include "AT4_System.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(AT4);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);

	System theSystem;
	if (!theSystem.init(argc, argv)) {
		return 0;
	}

	return theSystem.run();
}
