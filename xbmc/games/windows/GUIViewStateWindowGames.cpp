/*
 *      Copyright (C) 2012-2016 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "GUIViewStateWindowGames.h"
#include "games/GameManager.h"
#include "FileItem.h"
#include "guilib/GraphicContext.h" // include before ViewState.h
#include "guilib/LocalizeStrings.h"
#include "guilib/WindowIDs.h"
#include "input/Key.h"
#include "settings/MediaSourceSettings.h"
#include "settings/Settings.h"
#include "utils/StringUtils.h"
#include "view/ViewState.h"
#include "view/ViewStateSettings.h"

#include <assert.h>

using namespace GAME;

CGUIViewStateWindowGames::CGUIViewStateWindowGames(const CFileItemList& items) : CGUIViewState(items)
{
  if (items.IsVirtualDirectoryRoot())
  {
    AddSortMethod(SortByNone, 551, LABEL_MASKS("%F", "%I", "%L", ""));  // Filename, Size | Label, empty
    SetSortMethod(SortByNone);
    SetViewAsControl(DEFAULT_VIEW_LIST);
    SetSortOrder(SortOrderNone);
  }
  else
  {
    AddSortMethod(SortByFile, 561, LABEL_MASKS("%F", "%I", "%L", ""));  // Filename, Size | Label, empty
    AddSortMethod(SortBySize, 553, LABEL_MASKS("%L", "%I", "%L", "%I"));  // Filename, Size | Label, Size
    SetSortMethod(SortByFile);

    const CViewState *viewState = CViewStateSettings::GetInstance().Get("games");
    SetSortMethod(viewState->m_sortDescription);
    SetViewAsControl(viewState->m_viewMode);
    SetSortOrder(viewState->m_sortDescription.sortOrder);
  }

  LoadViewState(items.GetPath(), WINDOW_GAMES);
}

std::string CGUIViewStateWindowGames::GetLockType()
{
  return "games";
}

std::string CGUIViewStateWindowGames::GetExtensions()
{
  std::vector<std::string> exts;

  CGameManager::GetInstance().GetExtensions(exts);

  // Ensure .zip appears
  if (std::find(exts.begin(), exts.end(), ".zip") == exts.end())
    exts.push_back(".zip");

  return StringUtils::Join(exts, "|");
}

VECSOURCES& CGUIViewStateWindowGames::GetSources()
{
  VECSOURCES *pGameSources = CMediaSourceSettings::GetInstance().GetSources("games");

  // Guard against source type not existing
  assert(pGameSources != nullptr);

  // Game add-ons
  AddAddonsSource("game", g_localizeStrings.Get(35049), "DefaultAddonGame.png");

  // Global sources
  AddOrReplace(*pGameSources, CGUIViewState::GetSources());

  return *pGameSources;
  /*
  m_sources.clear();

  // Files
  {
    CMediaSource share;
    share.strPath = "sources://games/";
    share.strName = g_localizeStrings.Get(744); // Files
    share.m_iDriveType = CMediaSource::SOURCE_TYPE_LOCAL;
    share.m_strThumbnailImage = "DefaultFolder.png";
    m_sources.push_back(share);
  }

  // Add-ons
  AddAddonsSource("game", g_localizeStrings.Get(27016), "DefaultAddonGame.png"); // Game Add-ons
  return CGUIViewState::GetSources();
  */
}

void CGUIViewStateWindowGames::SaveViewState()
{
  SaveViewToDb(m_items.GetPath(), WINDOW_GAMES, CViewStateSettings::GetInstance().Get("games"));
}
