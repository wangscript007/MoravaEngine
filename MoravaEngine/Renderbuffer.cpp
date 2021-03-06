#include "Renderbuffer.h"

#include "Log.h"

#include <stdio.h>
#include <stdexcept>


Renderbuffer::Renderbuffer() : Attachment()
{
}

Renderbuffer::Renderbuffer(unsigned int width, unsigned int height, AttachmentFormat attachmentFormat, unsigned int orderID)
	: Attachment(width, height, AttachmentType::Renderbuffer, attachmentFormat, orderID)
{
	GLenum internalFormat;
	GLenum attachment;

	switch (attachmentFormat)
	{
	case AttachmentFormat::Depth:
		internalFormat = GL_DEPTH_COMPONENT;
		attachment = GL_DEPTH_ATTACHMENT;
		break;
	case AttachmentFormat::Depth_24:
		internalFormat = GL_DEPTH_COMPONENT24;
		attachment = GL_DEPTH_ATTACHMENT;
		break;
	case AttachmentFormat::Stencil:
		internalFormat = GL_STENCIL_INDEX;
		attachment = GL_STENCIL_ATTACHMENT;
		break;
	case AttachmentFormat::DepthStencil:
	case AttachmentFormat::Depth_24_Stencil_8:
		internalFormat = GL_DEPTH24_STENCIL8;
		attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		break;
	default:
		throw std::runtime_error("AttachmentFormat not supported!");
		return;
	}

	glGenRenderbuffers(1, &m_ID);
	Bind();
	// Create a depth and stencil renderbuffer object
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_Width, m_Height);
	// Attach the renderbuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_ID);
	Unbind();
	// Log::GetLogger()->info("Renderbuffer ID={0}, m_Width={1}, m_Height={2}", m_ID, m_Width, m_Height);
}

void Renderbuffer::Bind(unsigned int slot)
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void Renderbuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
	// Log::GetLogger()->info("Renderbuffer Destructor");

	glDeleteRenderbuffers(1, &m_ID);
}
