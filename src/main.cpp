#include <gtkmm.h>
#include <memory>

// Forward declaration of ChatWindow
class ChatWindow;

class EntryWindow : public Gtk::Window
{
public:
    EntryWindow(Glib::RefPtr<Gtk::Application> app);

private:
    Gtk::Box vbox;
    Gtk::Label label;
    Gtk::Entry entry;
    Gtk::Button btn;
    Glib::RefPtr<Gtk::Application> app;
    
    void on_button_clicked();
};

// Chat Window Class
class ChatWindow : public Gtk::Window
{
public:
    ChatWindow(const Glib::ustring& username);

private:
    Gtk::Box vbox;
    Gtk::Box hbox;
    Gtk::TextView textview_display;
    Gtk::ScrolledWindow scrolled_window_display;
    Gtk::TextView textview_input;
    Gtk::ScrolledWindow scrolled_window_input;
    Gtk::Button btn;
    Glib::RefPtr<Gtk::TextBuffer> buffer_display;
    Glib::ustring username;

    void on_button_clicked();
};

//  Entry Window Constructor (Collects Username)
EntryWindow::EntryWindow(Glib::RefPtr<Gtk::Application> app)
    : vbox(Gtk::ORIENTATION_VERTICAL, 10),
      label("Enter your username:"),
      btn("Submit"),
      app(app)
{
    set_title("Login");
    set_default_size(300, 150);

    // Align UI elements
    entry.set_placeholder_text("Type your username...");
    btn.set_margin_top(10);

    // Pack widgets
    vbox.pack_start(label, Gtk::PACK_SHRINK);
    vbox.pack_start(entry, Gtk::PACK_SHRINK);
    vbox.pack_start(btn, Gtk::PACK_SHRINK);

    // Connect event
    btn.signal_clicked().connect(sigc::mem_fun(*this, &EntryWindow::on_button_clicked));

    add(vbox);
    show_all_children();
}

//  When Submit is Clicked
void EntryWindow::on_button_clicked()
{
    Glib::ustring username = entry.get_text();

    if (!username.empty()) {
        // Create and show the chat window
        auto chat_window = new ChatWindow(username);
        chat_window->set_application(app);
        chat_window->show();

        // Hide the entry window and let GTK manage destruction
        hide();
    }
}

// Chat Window Constructor
ChatWindow::ChatWindow(const Glib::ustring& username)
    : vbox(Gtk::ORIENTATION_VERTICAL, 10),
      hbox(Gtk::ORIENTATION_HORIZONTAL, 10),
      btn("Send"),
      username(username)
{
    set_title("Chat - " + username);
    set_default_size(500, 300);

    // TextView for message display
    buffer_display = Gtk::TextBuffer::create();
    textview_display.set_buffer(buffer_display);
    textview_display.set_wrap_mode(Gtk::WRAP_CHAR);
    scrolled_window_display.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolled_window_display.add(textview_display);

    // TextView for input
    Glib::RefPtr<Gtk::TextBuffer> buffer_input = Gtk::TextBuffer::create();
    textview_input.set_buffer(buffer_input);
    textview_input.set_wrap_mode(Gtk::WRAP_CHAR);
    scrolled_window_input.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolled_window_input.add(textview_input);

    // Arrange input box and button
    hbox.set_size_request(400, 50);
    hbox.pack_start(scrolled_window_input, Gtk::PACK_EXPAND_WIDGET);
    hbox.pack_start(btn, Gtk::PACK_SHRINK);

    // Arrange elements in vertical layout
    vbox.pack_start(scrolled_window_display, Gtk::PACK_EXPAND_WIDGET);
    vbox.pack_start(hbox, Gtk::PACK_SHRINK);

    add(vbox);
    show_all_children();

    // Connect button event
    btn.signal_clicked().connect(sigc::mem_fun(*this, &ChatWindow::on_button_clicked));
}

//  When Send Button is Clicked
void ChatWindow::on_button_clicked()
{
    Glib::ustring new_text = textview_input.get_buffer()->get_text();

    if (!new_text.empty()) {
        // Append username and message to chat display
        Glib::ustring current_text = buffer_display->get_text();
        Glib::ustring updated_text = current_text + (current_text.empty() ? "" : "\n") + username + ": " + new_text;
        buffer_display->set_text(updated_text);

        // Auto-scroll to bottom
        Gtk::TextBuffer::iterator end_iter = buffer_display->end();
        buffer_display->place_cursor(end_iter);

        // Clear input box
        textview_input.get_buffer()->set_text("");
    }
}

// Main Function
int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.example.chatapp");
    EntryWindow window(app);
    return app->run(window);
}
