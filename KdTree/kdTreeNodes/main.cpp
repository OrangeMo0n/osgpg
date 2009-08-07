#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osg/Geometry>
#include "PointsDividor.h"
#include <fstream>
#include <iostream>

using namespace std;

/** Usage:
	appname.exe infile outfile
	*/
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "chs");
	if(argc<3) {
		cout<<"need infile and outfile name"<<endl;
	}

	try {
		std::fstream infile(argv[1]);
		FC::PointsDividor pd(infile, "test.log");
		if(!pd.process()) {
			cout<<"process error!"<<endl;
			return -1;
		}

		osg::ref_ptr<osg::LOD> root = pd.GetRoot();
		osgDB::writeNodeFile(*root, argv[2]);
	} catch (...) {
		cout<<"Error happens!"<<endl;
	}
	return 0;
}