/***************************************************************************
 *   Copyright (C) 2008-2010 by Andrzej Rybczak                            *
 *   electricityispower@gmail.com                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include "mpdpp.h"

#include "global.h"
#include "help.h"
#include "settings.h"
#include "tag_editor.h"

#include "i18n.h"

using Global::MainHeight;
using Global::MainStartY;

Help *myHelp = new Help;

void Help::Init()
{
	w = new Scrollpad(0, MainStartY, COLS, MainHeight, "", Config.main_color, brNone);
	GetKeybindings();
	w->Flush();
	isInitialized = 1;
}

void Help::Resize()
{
	w->Resize(COLS, MainHeight);
	w->MoveTo(0, MainStartY);
	hasToBeResized = 0;
}

void Help::SwitchTo()
{
	using Global::myScreen;
	
	if (myScreen == this)
		return;
	
	if (!isInitialized)
		Init();
	
	if (hasToBeResized)
		Resize();
	
	if (myScreen != this && myScreen->isTabbable())
		Global::myPrevScreen = myScreen;
	myScreen = this;
	Global::RedrawHeader = 1;

}

std::basic_string<my_char_t> Help::Title()
{
	return U("Help");
}

std::string Help::DisplayKeys(int *key, int size)
{
	bool backspace = 1;
	std::string result = "\t";
	for (int i = 0; i < size; i++)
	{
		if (key[i] == NcmpcppKeys::NullKey);
		else if (key[i] == 259)
			result += "Up";
		else if (key[i] == 258)
			result += "Down";
		else if (key[i] == 339)
			result += "Page Up";
		else if (key[i] == 338)
			result += "Page Down";
		else if (key[i] == 262)
			result += "Home";
		else if (key[i] == 360)
			result += "End";
		else if (key[i] == 32)
			result += "Space";
		else if (key[i] == 10)
			result += "Enter";
		else if (key[i] == 330)
			result += "Delete";
		else if (key[i] == 261)
			result += "Right";
		else if (key[i] == 260)
			result += "Left";
		else if (key[i] == 9)
			result += "Tab";
		else if (key[i] == 353)
			result += "Shift-Tab";
		else if (key[i] >= 1 && key[i] <= 26)
		{
			result += "Ctrl-";
			result += key[i]+64;
		}
		else if (key[i] >= 265 && key[i] <= 276)
		{
			result += "F";
			result += IntoStr(key[i]-264);
		}
		else if ((key[i] == 263 || key[i] == 127) && !backspace);
		else if ((key[i] == 263 || key[i] == 127) && backspace)
		{
			result += "Backspace";
			backspace = 0;
		}
		else
			result += key[i];
		result += " ";
	}
	if (result.length() > 12)
		result = result.substr(0, 12);
	for (size_t i = result.length(); i <= 12; result += " ", ++i) { }
	result += ": ";
	return result;
}

void Help::GetKeybindings()
{
	*w << "   " << fmtBold << _("Keys - Movement\n -----------------------------------------\n") << fmtBoldEnd;
	*w << DisplayKeys(Key.Up)			<< _("Move Cursor up\n");
	*w << DisplayKeys(Key.Down)			<< _("Move Cursor down\n");
	*w << DisplayKeys(Key.UpAlbum)			<< _("Move Cursor up one album\n");
	*w << DisplayKeys(Key.DownAlbum)		<< _("Move Cursor down one album\n");
	*w << DisplayKeys(Key.UpArtist)			<< _("Move Cursor up one artist\n");
	*w << DisplayKeys(Key.DownArtist)		<< _("Move Cursor down one artist\n");
	*w << DisplayKeys(Key.PageUp)			<< _("Page up\n");
	*w << DisplayKeys(Key.PageDown)			<< _("Page down\n");
	*w << DisplayKeys(Key.Home)			<< _("Home\n");
	*w << DisplayKeys(Key.End)			<< _("End\n");
	*w << "\n";
	if (Config.screen_switcher_previous)
		*w << DisplayKeys(Key.ScreenSwitcher)   << _("Switch between current and last screen\n");
	else
	{
		*w << DisplayKeys(Key.ScreenSwitcher)   << _("Switch to next screen in sequence\n");
		*w << DisplayKeys(Key.BackwardScreenSwitcher) << _("Switch to previous screen in sequence\n");
	}
	*w << DisplayKeys(Key.Help)			<< _("Help screen\n");
	*w << DisplayKeys(Key.Playlist)			<< _("Playlist screen\n");
	*w << DisplayKeys(Key.Browser)			<< _("Browse screen\n");
	*w << DisplayKeys(Key.SearchEngine)		<< _("Search engine\n");
	*w << DisplayKeys(Key.MediaLibrary)		<< _("Media library\n");
	*w << DisplayKeys(Key.PlaylistEditor)		<< _("Playlist editor\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.TagEditor)		<< _("Tag editor\n");
#	endif // HAVE_TAGLIB_H
#	ifdef ENABLE_OUTPUTS
	*w << DisplayKeys(Key.Outputs)			<< _("Outputs\n");
#	endif // ENABLE_OUTPUTS
#	ifdef ENABLE_VISUALIZER
	*w << DisplayKeys(Key.Visualizer)		<< _("Music visualizer\n");
#	endif // ENABLE_VISUALIZER
#	ifdef ENABLE_CLOCK
	*w << DisplayKeys(Key.Clock)			<< _("Clock screen\n");
#	endif // ENABLE_CLOCK
	*w << "\n";
	*w << DisplayKeys(Key.ServerInfo)		<< _("MPD server info\n");
	
	*w << "\n\n   " << fmtBold << "Keys - Global\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Stop)			<< _("Stop\n");
	*w << DisplayKeys(Key.Pause)			<< _("Pause\n");
	*w << DisplayKeys(Key.Next)			<< _("Next track\n");
	*w << DisplayKeys(Key.Prev)			<< _("Previous track\n");
	*w << DisplayKeys(Key.Replay)			<< _("Play current track from the beginning\n");
	*w << DisplayKeys(Key.SeekForward)		<< _("Seek forward\n");
	*w << DisplayKeys(Key.SeekBackward)		<< _("Seek backward\n");
	*w << DisplayKeys(Key.VolumeDown)		<< _("Decrease volume\n");
	*w << DisplayKeys(Key.VolumeUp)			<< _("Increase volume\n");
	*w << "\n";
	*w << DisplayKeys(Key.ToggleSpaceMode)		<< _("Toggle space mode (select/add)\n");
	*w << DisplayKeys(Key.ToggleAddMode)		<< _("Toggle add mode\n");
	*w << DisplayKeys(Key.ToggleMouse)		<< _("Toggle mouse support\n");
	*w << DisplayKeys(Key.ReverseSelection)		<< _("Reverse selection\n");
	*w << DisplayKeys(Key.DeselectAll)		<< _("Deselect all items\n");
	*w << DisplayKeys(Key.SelectAlbum)		<< _("Select songs of album around cursor\n");
	*w << DisplayKeys(Key.AddSelected)		<< _("Add selected items to playlist/m3u file\n");
	*w << "\n";
	*w << DisplayKeys(Key.ToggleRepeat)		<< _("Toggle repeat mode\n");
	*w << DisplayKeys(Key.ToggleRandom)		<< _("Toggle random mode\n");
	*w << DisplayKeys(Key.ToggleSingle)		<< _("Toggle single mode\n");
	*w << DisplayKeys(Key.ToggleConsume)		<< _("Toggle consume mode\n");
	if (Mpd.Version() >= 16)
		*w << DisplayKeys(Key.ToggleReplayGainMode)	<< _("Toggle replay gain mode\n");
	*w << DisplayKeys(Key.ToggleBitrateVisibility)	<< _("Toggle bitrate visibility\n");
	*w << DisplayKeys(Key.Shuffle)			<< _("Shuffle playlist\n");
	*w << DisplayKeys(Key.ToggleCrossfade)		<< _("Toggle crossfade mode\n");
	*w << DisplayKeys(Key.SetCrossfade)		<< _("Set crossfade\n");
	*w << DisplayKeys(Key.UpdateDB)			<< _("Start a music database update\n");
	*w << "\n";
	*w << DisplayKeys(Key.ApplyFilter)		<< _("Apply filter\n");
	*w << DisplayKeys(Key.FindForward)		<< _("Forward find\n");
	*w << DisplayKeys(Key.FindBackward)		<< _("Backward find\n");
	*w << DisplayKeys(Key.PrevFoundPosition)	<< _("Go to previous found position\n");
	*w << DisplayKeys(Key.NextFoundPosition)	<< _("Go to next found position\n");
	*w << DisplayKeys(Key.ToggleFindMode)		<< _("Toggle find mode (normal/wrapped)\n");
	*w << DisplayKeys(Key.GoToContainingDir)	<< _("Locate song in browser\n");
	*w << DisplayKeys(Key.GoToMediaLibrary)		<< _("Locate current song in media library\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.GoToTagEditor)		<< _("Locate current song in tag editor\n");
#	endif // HAVE_TAGLIB_H
	*w << DisplayKeys(Key.ToggleDisplayMode)	<< _("Toggle display mode\n");
	*w << DisplayKeys(Key.ToggleInterface)		<< _("Toggle user interface\n");
	*w << DisplayKeys(Key.ToggleSeparatorsInPlaylist) << _("Toggle displaying separators between albums\n");
	*w << DisplayKeys(Key.GoToPosition)		<< _("Go to given position in current song (in % by default)\n");
	*w << DisplayKeys(Key.SongInfo)			<< _("Show song info\n");
#	ifdef HAVE_CURL_CURL_H
	*w << DisplayKeys(Key.ArtistInfo)		<< _("Show artist info\n");
	*w << DisplayKeys(Key.ToggleLyricsDB)		<< _("Toggle lyrics database\n");
#	endif // HAVE_CURL_CURL_H
	*w << DisplayKeys(Key.Lyrics)			<< _("Show/hide song's lyrics\n");
	*w << "\n";
	*w << DisplayKeys(Key.Quit)			<< _("Quit\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Playlist\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Play\n");
	*w << DisplayKeys(Key.SwitchTagTypeList)	<< _("Add random songs/artists/albums to playlist\n");
	*w << DisplayKeys(Key.Delete)			<< _("Delete item/selected items from playlist\n");
	*w << DisplayKeys(Key.Clear)			<< _("Clear playlist\n");
	*w << DisplayKeys(Key.Crop)			<< _("Clear playlist but hold currently playing/selected items\n");
	*w << DisplayKeys(Key.MvSongUp)			<< _("Move item(s) up\n");
	*w << DisplayKeys(Key.MvSongDown)		<< _("Move item(s) down\n");
	*w << DisplayKeys(Key.MoveTo)			<< _("Move selected item(s) to cursor position\n");
	*w << DisplayKeys(Key.MoveBefore)		<< _("Move selected item(s) before cursor position\n");
	*w << DisplayKeys(Key.MoveAfter)		<< _("Move selected item(s) after cursor position\n");
	*w << DisplayKeys(Key.Add)			<< _("Add url/file/directory to playlist\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.EditTags)			<< _("Edit song's tags\n");
#	endif // HAVE_TAGLIB_H
	*w << DisplayKeys(Key.SavePlaylist)		<< _("Save playlist\n");
	*w << DisplayKeys(Key.SortPlaylist)		<< _("Sort/reverse playlist\n");
	*w << DisplayKeys(Key.GoToNowPlaying)		<< _("Go to currently playing position\n");
	*w << DisplayKeys(Key.ToggleAutoCenter)		<< _("Toggle auto center mode\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Browser\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Enter directory/Add item to playlist and play\n");
	*w << DisplayKeys(Key.Space)			<< _("Add item to playlist\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.EditTags)			<< _("Edit song's tags/Rename playlist/directory\n");
#	else
	*w << DisplayKeys(Key.EditTags)			<< _("Rename playlist/directory\n");
#	endif // HAVE_TAGLIB_H
	if (Mpd.GetHostname()[0] == '/') // are we connected to unix socket?
		*w << DisplayKeys(Key.Browser)		<< _("Browse MPD database/local filesystem\n");
	*w << DisplayKeys(Key.SwitchTagTypeList)	<< _("Toggle sort order\n");
	*w << DisplayKeys(Key.GoToNowPlaying)		<< _("Locate currently playing song\n");
	*w << DisplayKeys(Key.GoToParentDir)		<< _("Go to parent directory\n");
	*w << DisplayKeys(Key.Delete)			<< _("Delete playlist/file/directory\n");
	*w << DisplayKeys(Key.GoToContainingDir)	<< _("Jump to playlist editor (playlists only)\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Search engine\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Add item to playlist and play/change option\n");
	*w << DisplayKeys(Key.Space)			<< _("Add item to playlist\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.EditTags)			<< _("Edit song's tags\n");
#	endif // HAVE_TAGLIB_H
	*w << DisplayKeys(Key.ToggleSingle)		<< _("Start searching immediately\n");
	*w << DisplayKeys(Key.SearchEngine)		<< _("Reset search engine\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Media library\n -----------------------------------------\n" << fmtBoldEnd;
	if (!Config.media_library_disable_two_column_mode)
		*w << DisplayKeys(Key.MediaLibrary)	<< _("Switch between two/three columns\n");
	*w << DisplayKeys(Key.PrevColumn)		<< _("Previous column\n");
	*w << DisplayKeys(Key.NextColumn)		<< _("Next column\n");
	*w << DisplayKeys(Key.Enter)			<< _("Add to playlist and play song/album/artist's songs\n");
	*w << DisplayKeys(Key.Space)			<< _("Add to playlist song/album/artist's songs\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.EditTags)			<< _("Edit main tag/album/song's tags\n");
#	endif // HAVE_TAGLIB_H
	*w << DisplayKeys(Key.SwitchTagTypeList)	<< _("Tag type list switcher (left column)\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Playlist Editor\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.PrevColumn)		<< _("Previous column\n");
	*w << DisplayKeys(Key.NextColumn)		<< _("Next column\n");
	*w << DisplayKeys(Key.Enter)			<< _("Add item to playlist and play\n");
	*w << DisplayKeys(Key.Space)			<< _("Add to playlist/select item\n");
#	ifdef HAVE_TAGLIB_H
	*w << DisplayKeys(Key.EditTags)			<< _("Edit playlist's name/song's tags\n");
#	else
	*w << DisplayKeys(Key.EditTags)			<< _("Edit playlist's name\n");
#	endif // HAVE_TAGLIB_H
	*w << DisplayKeys(Key.MvSongUp)			<< _("Move item(s) up\n");
	*w << DisplayKeys(Key.MvSongDown)		<< _("Move item(s) down\n");
	*w << DisplayKeys(Key.Clear)			<< _("Clear current playlist\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Lyrics\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Space)			<< _("Switch for following lyrics of now playing song\n");
	*w << DisplayKeys(Key.EditTags)			<< _("Open lyrics in external editor\n");
	*w << DisplayKeys(Key.SwitchTagTypeList)	<< _("Refetch lyrics\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Artist info\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.SwitchTagTypeList)	<< _("Refetch artist info\n");
	
	
#	ifdef HAVE_TAGLIB_H
	*w << "\n\n   " << fmtBold << "Keys - Tiny tag editor\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Edit tag\n");
	*w << DisplayKeys(Key.ToggleSingle)		<< _("Save\n");
	
	
	*w << "\n\n   " << fmtBold << "Keys - Tag editor\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Change tag/filename for one song (left column)\n");
	*w << DisplayKeys(Key.Enter)			<< _("Perform operation on all/selected songs (middle column)\n");
	*w << DisplayKeys(Key.Space)			<< _("Switch to albums/directories view (left column)\n");
	*w << DisplayKeys(Key.Space)			<< _("Select/deselect song (right column)\n");
	*w << DisplayKeys(Key.PrevColumn)		<< _("Previous column\n");
	*w << DisplayKeys(Key.NextColumn)		<< _("Next column\n");
	*w << DisplayKeys(Key.GoToParentDir)		<< _("Go to parent directory (left column, directories view)\n");
#	endif // HAVE_TAGLIB_H
	
	
#	ifdef ENABLE_OUTPUTS
	*w << "\n\n   " << fmtBold << "Keys - Outputs\n -----------------------------------------\n" << fmtBoldEnd;
	*w << DisplayKeys(Key.Enter)			<< _("Enable/disable output\n");
#	endif // ENABLE_OUTPUTS
	
	
#	if defined(ENABLE_VISUALIZER) && defined(HAVE_FFTW3_H)
	*w << "\n\n   " << fmtBold << _("Keys - Music visualizer\n -----------------------------------------\n") << fmtBoldEnd;
	*w << DisplayKeys(Key.Space)			<< _("Toggle visualization type\n");
#	endif // ENABLE_VISUALIZER && HAVE_FFTW3_H
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Global\n -----------------------------------------\n") << fmtBoldEnd;
	*w << "\tLeft click on \"Playing/Paused\"	"	<< _(": Play/pause\n");
	*w << _("\tLeft click on progressbar	")		<< _(": Go to chosen position in played track\n");
	*w << "\n";
	*w << "\tMouse wheel on \"Volume: xx\"	"		<< _(": Change volume\n");
	*w << _("\tMouse wheel on main window	")		<< _(": Scroll\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Playlist\n -----------------------------------------\n") << fmtBoldEnd;
	*w << _("\tLeft click			")		<< _(": Highlight\n");
	*w << _("\tRight click			")		<< _(": Play\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Browser\n -----------------------------------------\n") << fmtBoldEnd;
	*w << _("\tLeft click on directory		")	<< _(": Enter directory\n");
	*w << _("\tRight click on directory	")		<< _(": Add to playlist\n");
	*w << "\n";
	*w << _("\tLeft click on song/playlist	")		<< _(": Add to playlist\n");
	*w << _("\tRight click on song/playlist	")		<< _(": Add to playlist and play\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Search engine\n -----------------------------------------\n") << fmtBoldEnd;
	*w << _("\tLeft click			")		<< _(": Highlight/switch value\n");
	*w << _("\tRight click			")		<< _(": Change value\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Media library\n -----------------------------------------\n") << fmtBoldEnd;
	*w << fmtBold << _("\tLeft/middle column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Highlight\n");
	*w << _("\t\tRight Click		")			<< _(": Add to playlist\n");
	*w << "\n";
	*w << fmtBold << _("\tRight column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Add to playlist\n");
	*w << _("\t\tRight Click		")			<< _(": Add to playlist and play\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Playlist editor\n -----------------------------------------\n") << fmtBoldEnd;
	*w << fmtBold << _("\tLeft column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Highlight\n");
	*w << _("\t\tRight Click		")			<< _(": Add to playlist\n");
	*w << "\n";
	*w << fmtBold << _("\tRight column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Add to playlist\n");
	*w << _("\t\tRight Click		")			<< _(": Add to playlist and play\n");
	
	
#	ifdef HAVE_TAGLIB_H
	*w << "\n\n   " << fmtBold << _("Mouse - Tiny tag editor\n -----------------------------------------\n") << fmtBoldEnd;
	*w << _("\tLeft click			")		<< _(": Highlight\n");
	*w << _("\tRight click			")		<< _(": Change value/execute command\n");
	
	
	*w << "\n\n   " << fmtBold << _("Mouse - Tag editor\n -----------------------------------------\n") << fmtBoldEnd;
	*w << fmtBold << _("\tLeft column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Enter directory/highlight album\n");
	*w << _("\t\tRight Click		")			<< _(": Switch to directories/albums view\n");
	*w << "\n";
	*w << fmtBold << _("\tMiddle column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Highlight\n");
	*w << _("\t\tRight Click		")			<< _(": Change value/execute command\n");
	*w << "\n";
	*w << fmtBold << _("\tRight column:\n") << fmtBoldEnd;
	*w << _("\t\tLeft Click		")			<< _(": Highlight\n");
	*w << _("\t\tRight Click		")			<< _(": Change value\n");
#	endif // HAVE_TAGLIB_H
	
	
#	ifdef ENABLE_OUTPUTS
	*w << "\n\n   " << fmtBold << _("Mouse - Outputs\n -----------------------------------------\n") << fmtBoldEnd;
	*w << _("\tLeft click			")		<< _(": Highlight\n");
	*w << _("\tRight click			")		<< _(": Enable/disable output\n");
#	endif // ENABLE_OUTPUTS
}

