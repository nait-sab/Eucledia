#pragma once

namespace Eucledia
{
	enum class ShaderDataType
	{
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4, 
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDateTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 2;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		EUCLEDIA_CORE_ASSERT(false, "Unknow ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType _type;
		std::string _name;
		uint32_t _size;
		size_t _offset;
		bool _normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: _type(type), _name(name), _size(ShaderDateTypeSize(type)), _offset(0), _normalized(normalized) {}

		uint32_t getComponentCount() const
		{
			switch (_type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			EUCLEDIA_CORE_ASSERT(false, "Unknow ShaderDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: _elements(elements)
		{
			calculateOffesetsAndStride();
		}

		inline uint32_t getStride() const { return _stride; }
		inline const std::vector<BufferElement>& getElements() const { return _elements; }

		std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

	private:
		std::vector<BufferElement> _elements;
		uint32_t _stride = 0;

		void calculateOffesetsAndStride()
		{
			size_t offset = 0;
			_stride = 0;

			for (auto& element : _elements)
			{
				element._offset = offset;
				offset += element._size;
				_stride += element._size;
			}
		}
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getlayout() const = 0;

		static ref<VertexBuffer> create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static ref<IndexBuffer> create(uint32_t* indices, uint32_t size);
	};
}