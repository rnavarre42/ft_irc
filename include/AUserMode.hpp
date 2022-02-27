#ifndef AUSERMODE_HPP
# define AUSERMODE_HPP
// Necesitamos una clase para controlar los modos de usuario
//
// +i invisible
// en who no aparece. Names oculta su nick si no se
// comparte canal. whois muestra su modo
// 
// +O ircop
// en who aparece remarcado con una O en sus modos
// whois muestra su modo y un numeric específico
//
// AUserMode debe almacenar un flag y un modo
// 

class AUserMode
{
public:
	AUserMode(void);
	virtual ~AUserMode(void) = 0;

	virtual void	onChanEvent();

protected:
	char		_mode;
	uint64_t	_flag;

}

#endif
