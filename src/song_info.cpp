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

#include "global.h"
#include "song_info.h"
#include "tag_editor.h"
#include "i18n.h"

using Global::MainHeight;
using Global::MainStartY;
using Global::myScreen;
using Global::myOldScreen;

SongInfo *mySongInfo = new SongInfo;

SongInfo::Metadata SongInfo::Tags[] =
{
 { 0,		&MPD::Song::GetTitle,		&MPD::Song::SetTitle		},
 { 0,		&MPD::Song::GetArtist,		&MPD::Song::SetArtist		},
 { 0,		&MPD::Song::GetAlbumArtist,	&MPD::Song::SetAlbumArtist	},
 { 0,		&MPD::Song::GetAlbum,		&MPD::Song::SetAlbum		},
 { 0,		&MPD::Song::GetDate,		&MPD::Song::SetDate		},
 { 0,		&MPD::Song::GetTrack,		&MPD::Song::SetTrack		},
 { 0,		&MPD::Song::GetGenre,		&MPD::Song::SetGenre		},
 { 0,		&MPD::Song::GetComposer,	&MPD::Song::SetComposer		},
 { 0,		&MPD::Song::GetPerformer,	&MPD::Song::SetPerformer	},
 { 0,		&MPD::Song::GetDisc,		&MPD::Song::SetDisc		},
 { 0,		&MPD::Song::GetComment,		&MPD::Song::SetComment		},
 { 0,			0,				0				}
};

void SongInfo::Init()
{
	w = new Scrollpad(0, MainStartY, COLS, MainHeight, "", Config.main_color, brNone);
	isInitialized = 1;
}

void SongInfo::InitTranslation()
{
	Tags[0].Name = _("Title");
	Tags[1].Name = _("Artist");
	Tags[2].Name = _("Album Artist");
	Tags[3].Name = _("Album");
	Tags[4].Name = _("Year");
	Tags[5].Name = _("Track");
	Tags[6].Name = _("Genre");
	Tags[7].Name = _("Composer");
	Tags[8].Name = _("Performer");
	Tags[9].Name = _("Disc");
	Tags[10].Name = _("Comment");
}

void SongInfo::Resize()
{
	w->Resize(COLS, MainHeight);
	w->MoveTo(0, MainStartY);
	hasToBeResized = 0;
}

my_string_t SongInfo::Title()
{
	return TO_WSTRING(_("Song info"));
}

void SongInfo::SwitchTo()
{
	if (myScreen == this)
		return myOldScreen->SwitchTo();
	
	if (!isInitialized)
		Init();
	
	MPD::Song *s = myScreen->CurrentSong();
	
	if (!s)
		return;
	
	if (hasToBeResized)
		Resize();
	
	myOldScreen = myScreen;
	myScreen = this;
	
	Global::RedrawHeader = 1;
	
	w->Clear();
	w->Reset();
	PrepareSong(*s);
	w->Flush();
}

void SongInfo::PrepareSong(MPD::Song &s)
{
#	ifdef HAVE_TAGLIB_H
	std::string path_to_file;
	if (s.isFromDB())
		path_to_file += Config.mpd_music_dir;
	path_to_file += s.GetFile();
	TagLib::FileRef f(path_to_file.c_str());
	if (!f.isNull())
		s.SetComment(f.tag()->comment().to8Bit(1));
#	endif // HAVE_TAGLIB_H
	
	*w << fmtBold << Config.color1 << _("Filename") << ": " << fmtBoldEnd << Config.color2 << s.GetName() << "\n" << clEnd;
	*w << fmtBold << _("Directory") << ": " << fmtBoldEnd << Config.color2;
	ShowTag(*w, s.GetDirectory());
	*w << "\n\n" << clEnd;
	*w << fmtBold << _("Length") << ": " << fmtBoldEnd << Config.color2 << s.GetLength() << "\n" << clEnd;
#	ifdef HAVE_TAGLIB_H
	if (!f.isNull())
	{
		*w << fmtBold << _("Bitrate") << ": " << fmtBoldEnd << Config.color2 << f.audioProperties()->bitrate() << " " << _("kbps") << "\n" << clEnd;
		*w << fmtBold << _("Sample rate") << ": " << fmtBoldEnd << Config.color2 << f.audioProperties()->sampleRate() << " " << _("Hz") << "\n" << clEnd;
		*w << fmtBold << _("Channels") << ": " << fmtBoldEnd << Config.color2 << (f.audioProperties()->channels() == 1 ? _("Mono") : _("Stereo")) << "\n" << clDefault;
	}
#	endif // HAVE_TAGLIB_H
	*w << clDefault;
	
	for (const Metadata *m = Tags; m->Name; ++m)
	{
		*w << fmtBold << "\n" << TO_WSTRING(m->Name) << ": " << fmtBoldEnd;
		ShowTag(*w, s.GetTags(m->Get));
	}
}

