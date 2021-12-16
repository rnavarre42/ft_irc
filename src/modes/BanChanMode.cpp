#include "BanChanMode.hpp"
#include "ChanModeConfig.hpp"

class User;
class Channel;

BanChanMode::BanChanMode(Server &server)
	: AChanMode(server)
{
	//Tenemos que suscribirnos a JOIN, PRIVMSG y NOTICE
	//
	//ban acepta un parámetro, por lo que tenemos que comunicarselo
	//al servidor para que nos lo envíe.
	//chanModeConfig.type = modeType.BothParam; // el parámetro se ha de tener en cuenta en +b y -b
	//chanModeConfig.type = modeType.enableParam;
	//chanModeConfig.type = modeType.disableParam;
	//chanModeConfig.mode = 'b';
	//chanModeConfig.events = JOIN_EVENT | PRIVMSG_EVENT | NOTICE_EVENT;

	this->_chanModeConfig.type = ChanModeConfig::bothParam;
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

void	BanChanMode::onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)value;
	(void)access;
	(void)user;
	(void)channel;
	//TODO: value contendría la máscara a añadir
	//nesitamos comprobar si cumple con lo que sería una máscara
	//ver si no existe
	//agregarla

	//En este caso, el modo +b es una lista de máscaras que admiten
	//wildcards.
	//ntpsm no acepta parámetro alguno y por tanto value == empty()
	//
	//ov aceptan por parámetro un nick que exista en el canal.
	//Aquí se gregaría la instancia User de ese nick.
	//chanModeMultimap acepta un void * para que cada modo almacene
	//la información que necesite. Después, cada modo, se encargará
	//de volver a dejar ese void * al tipo de dato original.
	//
	//Si el parámetro no cumpliese, access = deny;
}

void	BanChanMode::onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value)
{
	(void)access;
	(void)value;
	(void)user;
	(void)channel;
	//TODO: value contendría la máscara a eliminar
	//necesitamos comprobar si cumple con lo que sería una máscara
	//ver si no existe
	//borrarla

}

void	BanChanMode::onShowChanModeEvent(void)
{
}
