#include "basic_server.hxx"

//--------------------------------------------------------------
// TcpServerConnection

TcpServerConnection::TcpServerConnection(boost::asio::io_context &io_context)
    : mSocket(io_context)
{
    mInputBuffer.resize(INPUT_BUFFER_SIZE);
}

TcpServerConnection::~TcpServerConnection()
{
    boost::system::error_code ec;
    mSocket.cancel(ec); // Ignore error for now
}

void TcpServerConnection::requestReceive()
{
    if (mReceiveRequested || mIsStopped)
    {
        return;
    }

    mSocket.async_read_some(boost::asio::buffer(&mInputBuffer[0],
                                                mInputBuffer.size()),
                            [this](const boost::system::error_code& ec, std::size_t length)
                            {
                                onReceive(ec, length);
                            }
    );
    mReceiveRequested = true;
}

void TcpServerConnection::onReceive(const boost::system::error_code &ec, std::size_t length)
{
    mReceiveRequested = false;
    if (ec) {
        mIsStopped = true;
    }
    else {
        cbReceive(ec, length);
        requestReceive();
    }
}

//--------------------------------------------------------------
// TcpAcceptor

TcpAcceptor::TcpAcceptor(boost::asio::io_context &io)
    :mIO(io), mListeningSocket(io)
{
}

TcpAcceptor::~TcpAcceptor() = default;

void
TcpAcceptor::start(
        const boost::asio::ip::tcp::endpoint &ep_to_bind)
{
    if (mListeningSocket.is_open())
    {
        throw std::logic_error("TcpAcceptor: duplicated start()");
    }
    mListeningSocket.open(boost::asio::ip::tcp::v4());
    mListeningSocket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(
            true));
    mListeningSocket.bind(ep_to_bind);
    mListeningSocket.listen(1);
    requestAccept();
}

void
TcpAcceptor::requestAccept()
{
    if (mAcceptRequested) {
        return;
    }
    // Schedule next acceptance in proactive pattern.
    TcpServerConnection *new_conn = createConnectionObject(mIO);
    mNextConnection.reset(new_conn);
    mListeningSocket.async_accept(mNextConnection->getSocket(),
                                  [this]
                                          (const boost::system::error_code& error)
                                  {handleAccept(error);});
    mAcceptRequested = true;
}

void
TcpAcceptor::handleAccept(const boost::system::error_code &error)
{
    mAcceptRequested = false;
    std::unique_ptr<TcpServerConnection> new_conn =
            std::move(mNextConnection);

    requestAccept();

    if (!error) {
        new_conn->requestReceive();
        mConnectionHistory.push_back(std::move(new_conn));
    }
}
