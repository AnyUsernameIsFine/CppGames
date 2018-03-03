#pragma once

namespace Framework
{
	namespace Utility
	{
		template<void(*ctor)()>
		struct StaticConstructor
		{
			struct Constructor {
				Constructor() {
					ctor();
				} 
			};

			static Constructor c;
		};

		template<void(*ctor)()>
		typename StaticConstructor<ctor>::Constructor StaticConstructor<ctor>::c;
	}
}
