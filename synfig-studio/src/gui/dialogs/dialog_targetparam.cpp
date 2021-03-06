/* === S Y N F I G ========================================================= */
/*!	\file dialog_targetparam.cpp
**	\brief Implementation for the TargetParam Dialog
**
**	$Id$
**
**	\legal
**	Copyright (c) 2010 Carlos López González
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "dialogs/dialog_targetparam.h"

#include "general.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace studio;

/* === M A C R O S ========================================================= */

#define CUSTOM_VCODEC_DESCRIPTION _("Custom Video Codec")
#define CUSTOM_VCODEC _("write your video codec here")

/* === G L O B A L S ======================================================= */
//! Allowed video codecs
/*! \warning This variable is linked to allowed_video_codecs_description,
 *  if you change this you must change the other acordingly.
 *  \warning These codecs are linked to the filename extensions for
 *  mod_ffmpeg. If you change this you must change the others acordingly.
 */
const char* allowed_video_codecs[] =
{
	"flv", "h263p", "huffyuv", "libtheora", "libx264", "libx264-lossless",
	"mjpeg", "mpeg1video", "mpeg2video", "mpeg4", "msmpeg4",
	"msmpeg4v1", "msmpeg4v2", "wmv1", "wmv2", CUSTOM_VCODEC, NULL
};

//! Allowed video codecs description.
/*! \warning This variable is linked to allowed_video_codecs,
 *  if you change this you must change the other acordingly.
 */
const char* allowed_video_codecs_description[] =
{
	_("Flash Video (FLV) / Sorenson Spark / Sorenson H.263"),
	_("H.263+ / H.263-1998 / H.263 version 2"),
	_("Huffyuv / HuffYUV"),
	_("libtheora Theora"),
	_("H.264 / AVC / MPEG-4 AVC"),
	_("H.264 / AVC / MPEG-4 AVC (LossLess)"),
	_("MJPEG (Motion JPEG)"),
	_("raw MPEG-1 video"),
	_("raw MPEG-2 video"),
	_("MPEG-4 part 2. (XviD/DivX)"),
	_("MPEG-4 part 2 Microsoft variant version 3"),
	_("MPEG-4 part 2 Microsoft variant version 1"),
	_("MPEG-4 part 2 Microsoft variant version 2"),
	_("Windows Media Video 7"),
	_("Windows Media Video 8"),
	CUSTOM_VCODEC_DESCRIPTION,
	NULL
};
/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

/* === E N T R Y P O I N T ================================================= */

Dialog_TargetParam::Dialog_TargetParam(Gtk::Window &parent, synfig::TargetParam &tparam):
	Gtk::Dialog(_("Target Parameters"), parent)
{
	set_tparam(tparam);
	// Custom Video Codec Entry
	Gtk::Label* custom_label(manage(new Gtk::Label(std::string(CUSTOM_VCODEC_DESCRIPTION)+":")));
	custom_label->set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	customvcodec=Gtk::manage(new Gtk::Entry());
	// Available Video Codecs Combo Box Text.
	Gtk::Label* label(manage(new Gtk::Label(_("Available Video Codecs:"))));
	label->set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	vcodec = Gtk::manage(new Gtk::ComboBoxText());
	// Appends the codec descriptions to the Combo Box
	for (int i = 0; allowed_video_codecs[i] != NULL &&
					allowed_video_codecs_description[i] != NULL; i++)
		vcodec->append(allowed_video_codecs_description[i]);
	//Adds the Combo Box and the Custom Video Codec entry to the vertical box
	get_vbox()->pack_start(*label, true, true, 0);
	get_vbox()->pack_start(*vcodec, true, true, 0);
	get_vbox()->pack_start(*custom_label, true, true, 0);
	get_vbox()->pack_start(*customvcodec, true, true, 0);

	// Connect the signal change to the handler
	vcodec->signal_changed().connect(sigc::mem_fun(*this, &Dialog_TargetParam::on_vcodec_change));
	// By defaut, set the active text to the Custom Video Codec
	vcodec->set_active_text(CUSTOM_VCODEC_DESCRIPTION);
	customvcodec->set_text(CUSTOM_VCODEC);
	//Compare the passed vcodec to the available and set it active if found
	for (int i = 0; allowed_video_codecs[i] != NULL &&
					allowed_video_codecs_description[i] != NULL; i++)
		if(!get_tparam().video_codec.compare(allowed_video_codecs[i]))
		{
			vcodec->set_active_text(allowed_video_codecs_description[i]);
			customvcodec->set_text(allowed_video_codecs[i]);
		}

	//Bitrate Spin Button
	bitrate = Gtk::manage(
		new Gtk::SpinButton(
			Gtk::Adjustment::create(double(tparam.bitrate), 10.0,100000.0) ));
	Gtk::Label* label2(manage(new Gtk::Label(_("Video Bit Rate:"))));
	label2->set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
	get_vbox()->pack_start(*label2, true, true, 0);
	get_vbox()->pack_start(*bitrate,true, true, 0);

	get_vbox()->show_all();

	ok_button = manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
	ok_button->show();
	add_action_widget(*ok_button,Gtk::RESPONSE_OK);
	ok_button->signal_clicked().connect(sigc::mem_fun(*this,&Dialog_TargetParam::on_ok));

	cancel_button = manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
	cancel_button->show();
	add_action_widget(*cancel_button,Gtk::RESPONSE_CANCEL);
	cancel_button->signal_clicked().connect(sigc::mem_fun(*this,&Dialog_TargetParam::on_cancel));

}

void
Dialog_TargetParam::on_ok()
{
	tparam_.video_codec=customvcodec->get_text().c_str();
	tparam_.bitrate=bitrate->get_value();
	hide();
}

void
Dialog_TargetParam::on_cancel()
{
	hide();
}

void
Dialog_TargetParam::on_vcodec_change()
{
	std::string codecnamed = vcodec->get_active_text();
	customvcodec->set_sensitive(false);
	for (int i = 0; allowed_video_codecs[i] != NULL &&
					allowed_video_codecs_description[i] != NULL; i++)
		if(!codecnamed.compare(allowed_video_codecs_description[i]))
		{
			if(!codecnamed.compare(CUSTOM_VCODEC_DESCRIPTION))
				customvcodec->set_sensitive(true);
			else
				customvcodec->set_text(allowed_video_codecs[i]);
		}
}

Dialog_TargetParam::~Dialog_TargetParam()
{
}

