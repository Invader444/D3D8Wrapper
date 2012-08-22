/*
 * This file is part of the Direct3D8Wrapper Library
 * Copyright (C) 2012  Jonathan M. Horowitz
 *
 * Direct3D8Wrapper is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// dllmain.h : Declaration of module class.

class CD3D8WrapperModule : public CAtlDllModuleT< CD3D8WrapperModule >
{
public :
	DECLARE_LIBID(LIBID_D3D8WrapperLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_D3D8WRAPPER, "{C746DCBE-7F9C-4644-8455-AD641248D543}")
};

extern class CD3D8WrapperModule _AtlModule;
