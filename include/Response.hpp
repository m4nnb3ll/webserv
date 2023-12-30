/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asekkak <asekkak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:17:44 by mbouyahy          #+#    #+#             */
/*   Updated: 2023/12/30 12:02:23 by asekkak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <ft_common.h>
// #include "Client.hpp"

// class Client;
 

class Response
{
    public:
        Response();
        Response(const std::string value);
        // header line
        std::string                 _https_status_code;
        std::string                 _version_Http;
        // .add
        
        // headers      
        std::string                 _content_Type;
        std::string                 _content_Length;
        std::string                 _name_Server;
        // .add
        
        //body (check is body existe or not)      
        bool                        isBody;
        std::string                 _body;
        // .add
        
};

  
#endif