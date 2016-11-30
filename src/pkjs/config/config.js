var locales = {
    "en-US": require("./locales/en_US.json"),
    "ru-RU": require("./locales/ru_RU.json")
};

function _(string) {
    if(navigator.language in locales) {
        return locales[navigator.language][string];
    } else {
        if(string in locales["en-US"]) {
            return locales["en-US"][string];
        } else {
            return string;
        }
    }
}

module.exports = [
    {
        "type": "heading",
        "defaultValue": _("Metro")
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": _("Settings")
            },
            {
                "type": "select",
                "messageKey": "CITY_CHANGE",
                "defaultValue": 0,
                "label": _("City"),
                "options": [
                    {
                        "label": _("Moscow"),
                        "value": 0
                    },
                    {
                        "label": _("Saint-Petersburg"),
                        "value": 1
                    },
                    {
                        "label": _("Kiev"),
                        "value": 2
                    },
                    {
                        "label": _("Kharkiv"),
                        "value": 3
                    },
                    {
                        "label": _("Minsk"),
                        "value": 4
                    }
                ]
            },
            {
                "type": "select",
                "messageKey": "LOCALE_CHANGE",
                "defaultValue": 0,
                "label": _("Language"),
                "options": [
                    {
                        "label": _("English"),
                        "value": 0
                    },
                    {
                        "label": _("Russian"),
                        "value": 1
                    }
                ]
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": _("Save")
    }
];