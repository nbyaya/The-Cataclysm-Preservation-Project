/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "vmapexport.h"
#include "adtfile.h"
#include <algorithm>
#include <cstdio>
#include "Errors.h"

char const* GetPlainName(char const* FileName)
{
    const char * szTemp;

    if ((szTemp = strrchr(FileName, '\\')) != nullptr)
        FileName = szTemp + 1;
    return FileName;
}

char* GetPlainName(char* FileName)
{
    char * szTemp;

    if ((szTemp = strrchr(FileName, '\\')) != nullptr)
        FileName = szTemp + 1;
    return FileName;
}

void FixNameCase(char* name, size_t len)
{
    char* ptr = name + len - 1;

    //extension in lowercase
    for (; *ptr != '.'; --ptr)
        *ptr |= 0x20;

    for (; ptr >= name; --ptr)
    {
        if (ptr > name && *ptr >= 'A' && *ptr <= 'Z' && isalpha(*(ptr - 1)))
            *ptr |= 0x20;
        else if ((ptr == name || !isalpha(*(ptr - 1))) && *ptr >= 'a' && *ptr <= 'z')
            *ptr &= ~0x20;
    }
}

void FixNameSpaces(char* name, size_t len)
{
    if (len < 3)
        return;

    for (size_t i = 0; i < len - 3; i++)
        if (name[i] == ' ')
            name[i] = '_';
}

char* GetExtension(char* FileName)
{
    if (char* szTemp = strrchr(FileName, '.'))
        return szTemp;
    return nullptr;
}

extern HANDLE WorldMpq;

ADTFile::ADTFile(char* filename, bool cache) : _file(WorldMpq, filename, false)
{
    Adtfilename.append(filename);
    cacheable = cache;
    dirfileCache = nullptr;
}

bool ADTFile::init(uint32 map_num, uint32 originalMapId)
{
    if (dirfileCache)
        return initFromCache(map_num, originalMapId);

    if (_file.isEof ())
        return false;

    uint32 size;

    std::string xMap;
    std::string yMap;

    Adtfilename.erase(Adtfilename.find(".adt"),4);
    std::string TempMapNumber;
    TempMapNumber = Adtfilename.substr(Adtfilename.length()-6,6);
    xMap = TempMapNumber.substr(TempMapNumber.find("_")+1,(TempMapNumber.find_last_of("_")-1) - (TempMapNumber.find("_")));
    yMap = TempMapNumber.substr(TempMapNumber.find_last_of("_")+1,(TempMapNumber.length()) - (TempMapNumber.find_last_of("_")));
    Adtfilename.erase((Adtfilename.length()-xMap.length()-yMap.length()-2), (xMap.length()+yMap.length()+2));
    //string AdtMapNumber = xMap + ' ' + yMap + ' ' + GetPlainName((char*)Adtfilename.c_str());
    //printf("Processing map %s...\n", AdtMapNumber.c_str());
    //printf("MapNumber = %s\n", TempMapNumber.c_str());
    //printf("xMap = %s\n", xMap.c_str());
    //printf("yMap = %s\n", yMap.c_str());

    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    FILE* dirfile = fopen(dirname.c_str(), "ab");
    if (!dirfile)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    if (cacheable)
        dirfileCache = new std::vector<ADTOutputCache>();

    while (!_file.isEof())
    {
        char fourcc[5];
        _file.read(&fourcc,4);
        _file.read(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = _file.getPos() + size;

        if (!strcmp(fourcc,"MCIN"))
        {
        }
        else if (!strcmp(fourcc,"MTEX"))
        {
        }
        else if (!strcmp(fourcc,"MMDX"))
        {
            if (size)
            {
                char* buf = new char[size];
                _file.read(buf, size);
                char* p = buf;
                int t = 0;
                while (p < buf + size)
                {
                    std::string path(p);

                    char* s = GetPlainName(p);
                    FixNameCase(s, strlen(s));
                    FixNameSpaces(s, strlen(s));

                    ModelInstanceNames.emplace_back(s);

                    ExtractSingleModel(path);

                    p += strlen(p) + 1;
                }
                delete[] buf;
            }
        }
        else if (!strcmp(fourcc,"MWMO"))
        {
            if (size)
            {
                char* buf = new char[size];
                _file.read(buf, size);
                char* p = buf;
                int q = 0;
                while (p < buf + size)
                {
                    char* s = GetPlainName(p);
                    FixNameCase(s, strlen(s));
                    FixNameSpaces(s, strlen(s));

                    WmoInstanceNames.emplace_back(s);

                    p += strlen(p) + 1;
                }
                delete[] buf;
            }
        }
        //======================
        else if (!strcmp(fourcc, "MDDF"))
        {
            if (size)
            {
                uint32 doodadCount = size / sizeof(ADT::MDDF);
                for (uint32 i = 0; i < doodadCount; ++i)
                {
                    ADT::MDDF doodadDef;
                    _file.read(&doodadDef, sizeof(ADT::MDDF));
                    Doodad::Extract(doodadDef, ModelInstanceNames[doodadDef.Id].c_str(), map_num, originalMapId, dirfile, dirfileCache);
                }

                ModelInstanceNames.clear();
            }
        }
        else if (!strcmp(fourcc,"MODF"))
        {
            if (size)
            {
                uint32 mapObjectCount = size / sizeof(ADT::MODF);
                for (uint32 i = 0; i < mapObjectCount; ++i)
                {
                    ADT::MODF mapObjDef;
                    _file.read(&mapObjDef, sizeof(ADT::MODF));
                    MapObject::Extract(mapObjDef, WmoInstanceNames[mapObjDef.Id].c_str(), false, map_num, originalMapId, dirfile, dirfileCache);
                    Doodad::ExtractSet(WmoDoodads[WmoInstanceNames[mapObjDef.Id]], mapObjDef, false, map_num, originalMapId, dirfile, dirfileCache);
                }

                WmoInstanceNames.clear();
            }
        }

        //======================
        _file.seek(nextpos);
    }

    _file.close();
    fclose(dirfile);
    return true;
}

bool ADTFile::initFromCache(uint32 map_num, uint32 originalMapId)
{
    if (dirfileCache->empty())
        return true;

    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    FILE* dirfile = fopen(dirname.c_str(), "ab");
    if (!dirfile)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    for (ADTOutputCache const& cached : *dirfileCache)
    {
        fwrite(&map_num, sizeof(uint32), 1, dirfile);
        uint32 flags = cached.Flags;
        if (map_num != originalMapId)
            flags |= MOD_PARENT_SPAWN;
        fwrite(&flags, sizeof(uint32), 1, dirfile);
        fwrite(cached.Data.data(), cached.Data.size(), 1, dirfile);
    }

    fclose(dirfile);
    return true;
}

ADTFile::~ADTFile()
{
    _file.close();
    delete dirfileCache;
}
