#include "BanChanMode.hpp"
#include "ChanModeConfig.hpp"
#include "Message.hpp"
#include "Channel.hpp"

#include <ctime>

class User;
class Channel;

BanChanMode::BanChanMode(Server &server)
	: AChanMode(server)
{
	//Tenemos que suscribirnos a JOIN, PRIVMSG y NOTICE
	//
	//ban acepta un parámetro, por lo que tenemos que comunicarselo
	//al servidor para que nos lo envíe.

	this->_chanModeConfig.type = ChanModeConfig::enableParam | ChanModeConfig::disableParam;
	this->_chanModeConfig.mode = 'b';
	this->_chanModeConfig.events = CHANMODE_JOIN | CHANMODE_PRIVMSG | CHANMODE_NOTICE | CHANMODE_NICK;
}

BanChanMode::~BanChanMode(void) {}

void	BanChanMode::onChanEvent(Access &access, Message &message)
{
	(void)access;
	(void)message;
	//onChanEvent nos debe proporcionar un listado de las entradas
	//que coinciden con el modo que gestionamos, en esta ocasión 'b'
	//y verificar que la máscara de sender no está en el listado.
	//
	//en caso de que esté, se le deniega el comando, bien sea JOIN
	//PRIVMSG o NOTICE.
	//
	//lo único que cambiaría es el mensaje de error que es dstinto
	//en cada caso
	//
	//si se deniega, access = deny;
}

struct BanInfo
{
	BanInfo(std::string mask, std::string nick)
		: mask(mask), nick(nick), time(std::time(NULL))
	{}

	std::string	mask;
	std::string	nick;
	time_t		time;
};

inline Channel::Mode::multimap_iterator	findMask(Channel::Mode::rangePairMultimap_type rangePair, std::string mask)
{
	for (; rangePair.first != rangePair.second; ++rangePair.first)
	{
		if (static_cast<BanInfo *>(rangePair.first->second)->mask == mask)
			return rangePair.first;
	}
	return rangePair.second;
}

bool	BanChanMode::onChanModeEvent(int pos, int sign, Channel &channel, Message &message)
{
	BanInfo									*banInfo;
	Channel::Mode::rangePairMultimap_type	rangePair;
	Channel::Mode::multimap_iterator		maskIt;

	//TODO: verificar y completar mascara
	rangePair = channel.mode.getList(this->_chanModeConfig.mode);
	maskIt = findMask(rangePair, message[pos]);
	if (sign && maskIt == rangePair.second)
	{
		banInfo = new BanInfo(message[pos], message.getSender()->getName());
		this->setMode(channel, banInfo);
		return true;
	}
	else if (!sign && maskIt != rangePair.second)
	{
		this->unsetMode(channel, maskIt->second);
		return true;
	}
	return false;
}

void	BanChanMode::onShowChanModeEvent(void)
{
}

void	BanChanMode::onDelete(void *pointer)
{
	delete &*reinterpret_cast<BanInfo *>(pointer);
}
