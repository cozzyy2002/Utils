// UnitTest.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <log4cplus/configurator.h>

int main(int argc, TCHAR** argv)
{
	log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

	::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
