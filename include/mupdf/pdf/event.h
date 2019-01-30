#ifndef MUPDF_PDF_EVENT_H
#define MUPDF_PDF_EVENT_H

enum
{
	HOTSPOT_POINTER_DOWN = 0x1,
	HOTSPOT_POINTER_OVER = 0x2
};

/* Types of UI event */
enum
{
	PDF_EVENT_TYPE_POINTER,
};

/* Types of pointer event */
enum
{
	PDF_POINTER_DOWN,
	PDF_POINTER_UP,
};

/*
	UI events that can be passed to an interactive document.
*/
typedef struct pdf_ui_event_s
{
	int etype;
	union
	{
		struct
		{
			int ptype;
			fz_point pt;
		} pointer;
	} event;
} pdf_ui_event;

/*
	Set up a pointer event
*/
void pdf_init_ui_pointer_event(pdf_ui_event *event, int type, float x, float y);

/*
	Document events: the objects via which MuPDF informs the calling app
	of occurrences emanating from the document, possibly from user interaction
	or javascript execution. MuPDF informs the app of document events via a
	callback.
*/

/*
	Pass a UI event to an interactive
	document.

	Returns a boolean indication of whether the ui_event was
	handled. Example of use for the return value: when considering
	passing the events that make up a drag, if the down event isn't
	accepted then don't send the move events or the up event.
*/
int pdf_pass_event(fz_context *ctx, pdf_document *doc, pdf_page *page, pdf_ui_event *ui_event);

struct pdf_doc_event_s
{
	int type;
};

enum
{
	PDF_DOCUMENT_EVENT_ALERT,
	PDF_DOCUMENT_EVENT_PRINT,
	PDF_DOCUMENT_EVENT_LAUNCH_URL,
	PDF_DOCUMENT_EVENT_MAIL_DOC,
	PDF_DOCUMENT_EVENT_SUBMIT,
	PDF_DOCUMENT_EVENT_EXEC_MENU_ITEM,
	PDF_DOCUMENT_EVENT_EXEC_DIALOG
};

/*
	set the function via which to receive
	document events.
*/
void pdf_set_doc_event_callback(fz_context *ctx, pdf_document *doc, pdf_doc_event_cb *event_cb, void *data);
void *pdf_get_doc_event_callback_data(fz_context *ctx, pdf_document *doc);

/*
	The various types of document events
*/

/*
	details of an alert event. In response the app should
	display an alert dialog with the buttons specified by "button_type_group".
	If "check_box_message" is non-NULL, a checkbox should be displayed in
	the lower-left corned along with the message.

	"finally_checked" and "button_pressed" should be set by the app
	before returning from the callback. "finally_checked" need be set
	only if "check_box_message" is non-NULL.
*/
typedef struct
{
	const char *message;
	int icon_type;
	int button_group_type;
	const char *title;
	const char *check_box_message;
	int initially_checked;
	int finally_checked;
	int button_pressed;
} pdf_alert_event;

/* Possible values of icon_type */
enum
{
	PDF_ALERT_ICON_ERROR,
	PDF_ALERT_ICON_WARNING,
	PDF_ALERT_ICON_QUESTION,
	PDF_ALERT_ICON_STATUS
};

/* Possible values of button_group_type */
enum
{
	PDF_ALERT_BUTTON_GROUP_OK,
	PDF_ALERT_BUTTON_GROUP_OK_CANCEL,
	PDF_ALERT_BUTTON_GROUP_YES_NO,
	PDF_ALERT_BUTTON_GROUP_YES_NO_CANCEL
};

/* Possible values of button_pressed */
enum
{
	PDF_ALERT_BUTTON_NONE,
	PDF_ALERT_BUTTON_OK,
	PDF_ALERT_BUTTON_CANCEL,
	PDF_ALERT_BUTTON_NO,
	PDF_ALERT_BUTTON_YES
};

/*
	access the details of an alert event
	The returned pointer and all the data referred to by the
	structure are owned by mupdf and need not be freed by the
	caller.
*/
pdf_alert_event *pdf_access_alert_event(fz_context *ctx, pdf_doc_event *event);

/*
	access the details of am execMenuItem
	event, which consists of just the name of the menu item
*/
const char *pdf_access_exec_menu_item_event(fz_context *ctx, pdf_doc_event *event);

/*
	details of a launch-url event. The app should
	open the url, either in a new frame or in the current window.
*/
typedef struct
{
	const char *url;
	int new_frame;
} pdf_launch_url_event;

/*
	access the details of a launch-url
	event. The returned pointer and all data referred to by the structure
	are owned by mupdf and need not be freed by the caller.
*/
pdf_launch_url_event *pdf_access_launch_url_event(fz_context *ctx, pdf_doc_event *event);

/*
	details of a mail_doc event. The app should save
	the current state of the document and email it using the specified
	parameters.
*/
typedef struct
{
	int ask_user;
	const char *to;
	const char *cc;
	const char *bcc;
	const char *subject;
	const char *message;
} pdf_mail_doc_event;

/*
	access the details of a mail-doc event.
*/
pdf_mail_doc_event *pdf_access_mail_doc_event(fz_context *ctx, pdf_doc_event *event);

void pdf_event_issue_alert(fz_context *ctx, pdf_document *doc, pdf_alert_event *event);
void pdf_event_issue_print(fz_context *ctx, pdf_document *doc);
void pdf_event_issue_exec_menu_item(fz_context *ctx, pdf_document *doc, const char *item);
void pdf_event_issue_exec_dialog(fz_context *ctx, pdf_document *doc);
void pdf_event_issue_launch_url(fz_context *ctx, pdf_document *doc, const char *url, int new_frame);
void pdf_event_issue_mail_doc(fz_context *ctx, pdf_document *doc, pdf_mail_doc_event *event);

void pdf_execute_action(fz_context *ctx, pdf_document *doc, pdf_obj *obj, pdf_obj *a);

#endif
