#include "stdafx.h"
#include "CMap.h"

CMap::CMap(CWindow* window, std::string fileName) 
	: CResourceDirectories()
{
	m_pWindow = window;

	m_tileSetName = fileName;

	m_tileLength = 0;

	isMapLoaded = false;
	load();
}


CMap::~CMap()
{

}


void CMap::load()
{
	using namespace std;

	string filePath = m_resource_tileSets + m_tileSetName;

	ifstream fileStream;
	fileStream.open(filePath);
#ifdef DEBUG
	assert(fileStream.is_open());
#endif // DEBUG

	// flags to look for in the file
	string width		= "width=";
	string height		= "height=";
	string tileWidth	= "tilewidth=";
	string tileHeight	= "tileheight=";
	string tileSet		= "tileset=";
	string data			= "data=";

	// to stop double findings
	bool widthFound			= false;
	bool heightFound		= false;
	bool tileHeightFound	= false;
	bool tileWidthFound		= false;
	bool tileSetfound		= false;
	bool dataFound			= false;


	string line;
	while (getline(fileStream, line))
	{
		if (line.find(width) != string::npos && !widthFound)
		{
			string numStr = line.substr(width.length(), line.length());
			m_MapColumns = stringToInt(numStr);

			widthFound = true;
		}

		if (line.find(height) != string::npos && !heightFound)
		{
			string numStr = line.substr(height.length(), line.length());
			m_MapRows = stringToInt(numStr);

			heightFound = true;
		}

		if (line.find(tileWidth) != string::npos && !tileWidthFound)
		{
			string numStr = line.substr(tileWidth.length(), line.length());
			m_tileWidth = stringToInt(numStr);

			tileWidthFound = true;
		}

		if (line.find(tileHeight) != string::npos && !tileHeightFound)
		{
			string numStr = line.substr(tileHeight.length(), line.length());
			m_tileHeight = stringToInt(numStr);

			tileHeightFound = true;
		}

		if (line.find(tileSet) != string::npos && !tileSetfound)
		{
			string s = line.substr(tileSet.length(), line.length());
			string delimiter = ",";
			string name = s.substr(0, s.find(delimiter));
			m_tileSetName = name;
		}

		if (line.find(data) != string::npos && !dataFound)
		{

			for (int i = 0; i < m_MapRows; ++i)
			{
				getline(fileStream, line);

				vector<int> lineVector;
				int lineLength = line.length();
				for (int n = 0; n < lineLength; ++n)
				{
					string delimiter = ",";
					string numStr = line.substr(0, line.find(delimiter));
					int numInt = stringToInt(numStr);
					lineVector.push_back(numInt);

					// this line of code slowly reduces the size of the string
					//		each time a number is found (from the start to the next ',')
					//		it makes a substring from the rest of the line after the ','
					line = line.substr(line.find(delimiter)+1, line.length());
				}
				// get rid of any spot that was grown by the vector that we don't want
				//		(it is uninitialized data which could be a problem)
				lineVector.resize(m_MapColumns);
				
				m_mapData.push_back(lineVector);
			}

		}

	}

 	fileStream.close();

	isMapLoaded = true;

	m_pSpriteSheet = new CSprite(m_pWindow, m_tileSetName, 
		m_tileWidth, m_tileHeight, 
		1, 8);

	int spriteColumns = m_pSpriteSheet->getNumColumns();
	m_pTileCoordsArray = new SCoords2i[spriteColumns];

	// this for loop assumes that it is a 1D tile set of sequential indecies converted to lengths
	for (int i = 0; i < spriteColumns; ++i)
	{
		m_pTileCoordsArray[i] = SCoords2i(i+1, 1);
	}

	//// varifiying the coords
	//for (int i = 0; i < spriteColumns; ++i)
	//{
	//	SCoords2i cord = m_pTileCoordsArray[i];
	//	int p = 0;
	//}
}


void CMap::render()
{
	// using int = 0 instead of iterators because I need indecies of locations ([i][n])
	for (int i = 0; i < m_mapData.size(); ++i) // row
	{
		std::vector<int> lineVector = m_mapData[i];
		for (int n = 0; n < lineVector.size(); ++n) // column
		{
			// 0 represents no tile (thats why all sprite rendering is done in lengths)
			if (lineVector[n] == 0)
			{
				continue;
			}
			else
			{
				int index = lineVector[n];
				int screenX = n * m_tileWidth;
				int screenY = i * m_tileHeight;
				int screenW = m_tileWidth;
				int screenH = m_tileHeight;
				int spriteR = m_pTileCoordsArray[index-1].y;
				int spriteC = m_pTileCoordsArray[index-1].x;
				m_pSpriteSheet->render(screenX, screenY, screenW, screenH, 
					spriteR, spriteC);
			}
		}
	}
}


int CMap::stringToInt(std::string str)
{
	int result;
	std::stringstream convert(str);
	convert >> result;
	return result;
}


void CMap::free()
{
	if (isMapLoaded)
	{
		delete m_pSpriteSheet;
		delete[] m_pTileCoordsArray;
	}
}