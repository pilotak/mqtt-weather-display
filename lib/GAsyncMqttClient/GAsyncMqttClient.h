#include "AsyncMqttClient.h"
#include <memory>

class GAsyncMqttClient: public AsyncMqttClient {
  public:
    using msg_buff = std::shared_ptr<char>;
    GAsyncMqttClient& onGMessage(AsyncMqttClientInternals::OnMessageUserCallback callback) {
        onMessage([this, callback](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len,
        size_t index, size_t total) {
            if (len < total) {
                if (!m_pTempMesage) {
                    m_dwCursor = 0;
                    m_pTempMesage.reset(new msg_buff::element_type[total]);
                    memcpy((m_pTempMesage.get() + index), payload, len);
                    m_dwCursor += len;

                } else {
                    memcpy((m_pTempMesage.get() + index), payload, len);
                    m_dwCursor += len;

                    if (m_dwCursor == total) {
                        callback(topic, m_pTempMesage.get(), properties, m_dwCursor, 0, total);
                        m_pTempMesage.reset();
                    }
                }

            } else {
                callback(topic, payload, properties, len, index, total);
            }

        });
        return *this;
    }
  protected:
    msg_buff m_pTempMesage;
    uint32_t m_dwCursor;
};
