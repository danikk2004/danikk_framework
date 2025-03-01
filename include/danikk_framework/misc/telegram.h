#pragma once
#include <tgbot/tgbot.h>
#include <danikk_framework/string.h>
#include <danikk_framework/log.h>

namespace danikk_framework
{
	namespace telegram
	{
		namespace internal
		{
			using namespace TgBot;

			class Bot;

			template<class KeyboardReply_t, class KeyboardButton_t> class BaseReplyKeyboardMarkup : public KeyboardReply_t
			{
			private:
				friend class Bot;
			public:
				typedef std::shared_ptr<BaseReplyKeyboardMarkup<KeyboardReply_t, KeyboardButton_t>> Ptr;

				BaseReplyKeyboardMarkup()
				{
					//reply = std::make_shared<KeyboardReply_t>();
					if constexpr(std::is_same_v<KeyboardReply_t, ReplyKeyboardMarkup>)
					{
						this->oneTimeKeyboard = false;
						this->resizeKeyboard = true;
					}
				}

				BaseReplyKeyboardMarkup(cref<DynamicArray<String>> array)
				{
					set1columnKeyboard(array);
				}

				typedef typename KeyboardButton_t::Ptr KeyboardButtonPtr_t;

				static inline void addButtonToKeyboardLayer(ref<std::vector<KeyboardButtonPtr_t>> layer, cref<String> text, cref<String> callback_data = "")
				{
					KeyboardButtonPtr_t button = std::make_shared<KeyboardButton_t>();
					button->text = text.c_string();
					if constexpr(std::is_same_v<KeyboardButton_t, TgBot::InlineKeyboardButton>)
					{
						button->callbackData = callback_data;
					}
					layer.push_back(button);
				}

				auto& getKeyboard()
				{
					if constexpr(std::is_same_v<KeyboardReply_t, ReplyKeyboardMarkup>)
					{
						return this->keyboard;
					}
					else
					{
						return this->inlineKeyboard;
					}
				}

				inline void set1columnKeyboard(cref<DynamicArray<String>> array)
				{
					clear();
					for(cref<String> text : array)
					{
						insertButton(text);
					}
				}

				inline void set2columnKeyboard(cref<DynamicArray<String>> array)
				{
					set2columnKeyboard(array, [](cref<String> value){ return value;});
				}

				template<class array_value_t> inline void set2columnKeyboard(cref<DynamicArray<array_value_t>> array, auto get_str)
				{
					clear();
					cptr<array_value_t> currentRoomType = array.begin();
					cptr<array_value_t> endRoomType = array.end();
					while(currentRoomType < endRoomType)
					{
						cref<String> type1 = get_str(*currentRoomType++);
						if(currentRoomType < endRoomType)
						{
							cref<String> type2 =  get_str(*currentRoomType++);
							insertButtons(type1, type2);
						}
						else
						{
							insertButton(type1);
						}
					}
				}

				inline void insertButton(cref<String> text)
				{
					std::vector<KeyboardButtonPtr_t> button_layer = std::vector<KeyboardButtonPtr_t>();
					addButtonToKeyboardLayer(button_layer, text);
					getKeyboard().push_back(button_layer);
				}

				inline void insertInlineButton(cref<String> text, cref<String> callback_data)
				{
					std::vector<KeyboardButtonPtr_t> button_layer = std::vector<KeyboardButtonPtr_t>();
					addButtonToKeyboardLayer(button_layer, text, callback_data);
					getKeyboard().push_back(button_layer);
				}

				inline void insertButtons(cref<String> text1, cref<String> text2)
				{
					std::vector<KeyboardButtonPtr_t> button_layer = std::vector<KeyboardButtonPtr_t>();
					addButtonToKeyboardLayer(button_layer, text1);
					addButtonToKeyboardLayer(button_layer, text2);
					getKeyboard().push_back(button_layer);
				}

				inline void insertButtons(cref<String> text1, cref<String> text2, cref<String> text3)
				{
					std::vector<KeyboardButtonPtr_t> button_layer = std::vector<KeyboardButtonPtr_t>();
					addButtonToKeyboardLayer(button_layer, text1);
					addButtonToKeyboardLayer(button_layer, text2);
					addButtonToKeyboardLayer(button_layer, text3);
					getKeyboard().push_back(button_layer);
				}

				inline void clear()
				{
					getKeyboard().clear();
				}

				#undef getKeyboard
			};

			class Bot
			{
			private:

				std::unique_ptr<TgBot::Bot> bot;
				size_t is_work = false;

				typedef void(*message_event)(const TgBot::Message::Ptr message);
			public:
				Bot() = default;

				Bot(ccstring token)
				{
					bot = std::make_unique<TgBot::Bot>(token);
				}

				inline void setAnyMessageEvent(message_event listener)
				{
					bot->getEvents().onAnyMessage(listener);
				}

				inline void setCallbackQuery(auto listener)
				{
					bot->getEvents().onCallbackQuery(listener);
				}

				inline void sendMessage(cref<int64> chat_id, cref<String> text)
				{
					bot->getApi().sendMessage(chat_id, text, false, 0, std::make_shared<GenericReply>());
					formatLogInfo("В чат с id % было отправлено сообщение \"%\".", chat_id, text);
				}

				inline void sendMessage(cref<String> chat_id, cref<String> text)
				{
					bot->getApi().sendMessage(chat_id, text, false, 0, std::make_shared<GenericReply>());
					formatLogInfo("В чат с id % было отправлено сообщение \"%\".", chat_id, text);
				}

				inline void sendMessage(cref<int64> chat_id, cref<String> text, GenericReply::Ptr reply)
				{
					bot->getApi().sendMessage(chat_id, text, false, 0, reply , "html");
					formatLogInfo("В чат с id % было отправлено сообщение \"%\".", chat_id, text);
				}

				inline void sendPhoto(cref<int64> chat_id, cref<String> ref, cref<String> text)
				{
					bot->getApi().sendPhoto(chat_id, ref, text);
					formatLogInfo("В чат с id % было отправлено сообщение \"%\" с изображением \"%\".", chat_id, text, ref);
				}

				inline void sendPhoto(cref<int64> chat_id, cref<String> ref, cref<String> text, GenericReply::Ptr reply)
				{
					bot->getApi().sendPhoto(
						chat_id,
						(std::string&)ref,
						(std::string&)text,
						0,
						reply,
						"",
						false,
						std::vector<MessageEntity::Ptr>(),
						false);
					formatLogInfo("В чат с id % было отправлено сообщение \"%\" с изображением \"%\".", chat_id, text);
				}

				/*inline void sendMediaGroup(cref<int64> chat_id, cref<String> ref, cref<String> text, GenericReply::Ptr reply)
				{
					bot->getApi().sendMediaGroup(
						chat_id,
						media,
						false,
						0,
						false);
				}*/

				void run()
				{
					logInfo("Запуск бота.");
					is_work = true;

					while(is_work)
					{
						try
						{
							TgBot::TgLongPoll longPoll(*bot);
							while (is_work)
							{
								longPoll.start();
							}
						}
						catch (std::exception& error)
						{
							formatLogError("Ошибка telegram api: %", error.what());
						}
					}
				}

				void stop()
				{
					is_work = false;
				}
			};
		}

		using ReplyKeyboardMarkup = internal::BaseReplyKeyboardMarkup<TgBot::ReplyKeyboardMarkup, TgBot::KeyboardButton>;
		using InlineKeyboardMarkup = internal::BaseReplyKeyboardMarkup<TgBot::InlineKeyboardMarkup, TgBot::InlineKeyboardButton>;
		using GenericReply = TgBot::GenericReply;
		using CallbackQuery = TgBot::CallbackQuery;
		using internal::Message;
		using internal::Bot;
	}
}
