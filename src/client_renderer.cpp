#include "client_renderer.h"
#include "client_app.h"
#include <sstream>
#include <string>

#include "include/cef_dom.h"

namespace client_renderer {

const char kFocusedNodeChangedMessage[] = "ClientRenderer.FocusedNodeChanged";

namespace {

class ClientRenderDelegate : public ClientApp::RenderDelegate {
 public:
  ClientRenderDelegate()
    : last_node_is_editable_(false) {
  }

  virtual void OnFocusedNodeChanged(CefRefPtr<ClientApp> app,
                                    CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefDOMNode> node) OVERRIDE {
    bool is_editable = (node.get() && node->IsEditable());
    if (is_editable != last_node_is_editable_) {
      // Notify the browser of the change in focused element type.
      last_node_is_editable_ = is_editable;
      CefRefPtr<CefProcessMessage> message =
          CefProcessMessage::Create(kFocusedNodeChangedMessage);
      message->GetArgumentList()->SetBool(0, is_editable);
      browser->SendProcessMessage(PID_BROWSER, message);
    }
  }

 private:
  bool last_node_is_editable_;

  IMPLEMENT_REFCOUNTING(ClientRenderDelegate)
};

}  // namespace

void CreateRenderDelegates(ClientApp::RenderDelegateSet& delegates) {
  delegates.insert(new ClientRenderDelegate);
}

}  // namespace client_renderer
